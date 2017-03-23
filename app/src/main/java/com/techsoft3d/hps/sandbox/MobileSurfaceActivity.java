package com.techsoft3d.hps.sandbox;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.DownloadManager;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.view.inputmethod.EditorInfo;

public class MobileSurfaceActivity extends Activity implements
		AndroidMobileSurfaceView.Callback {

	private AndroidUserMobileSurfaceView mSurfaceView;

	private String mPath = "";
	private boolean mShouldLoadFile = false;
	private ProgressDialog mProgress;

	private boolean mFileNeedsDownload = false;

	private boolean mModeSimpleShadowEnabled;

	private FrameLayout mMainLayout;
	private View mCurrentToolbarView;
	private View mKeyboardTriggerView;

	BroadcastReceiver mDownloadBroadcastReceiver = null;

	private long mDownloadedFileId;

	// We only have 1 AndroidMobileSurfaceView in this app
	static final int MOBILE_SURFACE_GUI_ID = 0;

	// String used to store pointer when activity needs to save state
	// (e.g. Home screen was pressed, switched to another app)
	static final String MOBILE_SURFACE_POINTER_KEY = "mobileSurfaceId";

	private String getFilenameForContentUri() {
		Intent intent = getIntent();
		String attachmentFileName = "unknown";

		if (intent != null && intent.getData() != null) {
			Cursor cursor = getContentResolver().query(intent.getData(),
					new String[] { MediaStore.MediaColumns.DISPLAY_NAME },
					null, null, null);
			cursor.moveToFirst();
			final int index = cursor
					.getColumnIndex(MediaStore.MediaColumns.DISPLAY_NAME);
			if (index >= 0)
				attachmentFileName = cursor.getString(index);
			cursor.close();
		}

		return attachmentFileName;
	}
	
	private boolean mustDownloadFile(Bundle bundle, Uri uri){
		if (bundle != null) return false; // we are restoring a previous view..
		
		File file = new File(uri.getPath());
		return !file.exists();
	}

	private void downloadFileFromUri(Uri uri){
		mDownloadBroadcastReceiver = new BroadcastReceiver() {

			public void onReceive(Context ctxt, Intent intent) {
		    	String action = intent.getAction();
		    	DownloadManager downloadManager = (DownloadManager) getSystemService(Context.DOWNLOAD_SERVICE);
		    	
		        if (action.equals(DownloadManager.ACTION_DOWNLOAD_COMPLETE) )
		        	processCompletedDownload(downloadManager.getUriForDownloadedFile(mDownloadedFileId));
		    }
		};
		
		File file = new File(uri.getPath());
		String name = file.getName();
		
		registerReceiver(mDownloadBroadcastReceiver, new IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE));
		
		DownloadManager.Request request = new DownloadManager.Request(uri);
		request.setDestinationUri(Uri.fromFile(new File(Environment.getExternalStorageDirectory().getPath() + '/' + name)));
		
		DownloadManager manager = (DownloadManager) getSystemService(Context.DOWNLOAD_SERVICE);
		mDownloadedFileId = manager.enqueue(request);
	}
	
	private void processCompletedDownload(Uri uri){
		if (uri == null)
			return;
		
		File file = new File(uri.getPath());
		String name = file.getName();
		
		mPath = ViewerUtils.MY_DOCUMENTS_PATH + '/' + name;
		ViewerUtils.CopyFile(file.getPath(), mPath);
		file.delete();
		mFileNeedsDownload = false;
		
		Toast.makeText(getApplicationContext(), name + " Added to My Documents", Toast.LENGTH_SHORT).show();
		new LoadFileAsyncTask().execute(mPath);
	}	
	
	private void copyFileIfNecessary() {
		File file = new File(mPath);

		if (mPath.contains(ViewerUtils.MY_DOCUMENTS_PATH)
				|| mPath.contains(ViewerUtils.SAMPLE_DOCUMENTS_PATH)
				|| ViewerUtils.filenameInDirectory(file.getName(),
						ViewerUtils.MY_DOCUMENTS_PATH))
			return;

		String dst = ViewerUtils.MY_DOCUMENTS_PATH + '/' + file.getName();
		ViewerUtils.CopyFile(file.getPath(), dst);
		Toast.makeText(getApplicationContext(), 
				file.getName() + " Added to My Documents", Toast.LENGTH_SHORT).show();
	}



	private void processContentUri(Uri uri) {
		String filename = getFilenameForContentUri();

		mPath = ViewerUtils.MY_DOCUMENTS_PATH + '/' + filename;

		try {
			InputStream stream = getContentResolver().openInputStream(uri);
			ViewerUtils.CopyFile(stream, mPath);
		} catch (IOException e) {
		}

		Toast.makeText(getApplicationContext(),
				filename + " Added to My Documents", Toast.LENGTH_SHORT).show();
	}


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		ViewerUtils.LoadNativeLibs();
		// Store File passed in from intent
		Intent intent = getIntent();
		mPath = intent.getData().getPath();
		Uri uri = intent.getData();
		mShouldLoadFile = true;
		if (uri.getScheme().equalsIgnoreCase("content")){
			processContentUri(uri);
		}
		else{
			mFileNeedsDownload = mustDownloadFile(savedInstanceState, uri);
			if (!mFileNeedsDownload)
				copyFileIfNecessary();
			else{
				downloadFileFromUri(uri);
				mShouldLoadFile = false;
			}
		}

		Context context = getApplication();

		// If we're returning from a saved state, don't reload the file - just
		// set the surface pointer which we saved in the Bundle instance state
		// Otherwise, we going to load a file, so start the progress dialog.

		long mobileSurfacePointer = 0;

		if (savedInstanceState != null) {
			mobileSurfacePointer = savedInstanceState.getLong(MOBILE_SURFACE_POINTER_KEY);
			mShouldLoadFile = false;
		} else {
			mProgress = ProgressDialog.show(MobileSurfaceActivity.this, "", "Loading. Please wait...", true);
		}

		// Create our AndroidUserMobileSurfaceView using with our saved surface pointer, or 0 to indicate
		// it's a new instance.
		mSurfaceView = new AndroidUserMobileSurfaceView(context, this, MOBILE_SURFACE_GUI_ID, mobileSurfacePointer);

		// Setup our action bar with the file name and back button.
		ActionBar bar = getActionBar();
		File file = new File(mPath);
		bar.setTitle(file.getName());
		bar.setDisplayHomeAsUpEnabled(true);

		// Load operators toolbar as the default
		mCurrentToolbarView = getLayoutInflater().inflate(R.layout.operators, null);
		mKeyboardTriggerView = getLayoutInflater().inflate(R.layout.keyboard_trigger, null);
		 
		// The content view of this activity will be a FrameLayout (ViewGroup)
		// which contains the AndroidUserMobileSurfaceView, and a toolbar on top
		mMainLayout = new FrameLayout(context);
		mMainLayout.addView(mKeyboardTriggerView);
		mMainLayout.addView(mSurfaceView);
		mMainLayout.addView(mCurrentToolbarView);
		setContentView(mMainLayout);
		
		final EditTextWithBackListener hidden_text = (EditTextWithBackListener)mKeyboardTriggerView.findViewById(R.id.hidden_text_field);
		hidden_text.setSurfaceActivity(this);
		hidden_text.setSurfaceView(mSurfaceView);
		hidden_text.addTextChangedListener(new TextWatcher()
		{
			@Override
			public void onTextChanged(CharSequence s, int start, int before, int count)
			{
			}
			
			@Override
			public void beforeTextChanged(CharSequence s, int start, int count, int after)
			{
			}
			
			@Override
			public void afterTextChanged(Editable s)
			{
				mSurfaceView.onTextInput(hidden_text.getText().toString());
			}
		});
	}
	
	@Override
	public void onSaveInstanceState(Bundle savedInstanceState) {
		super.onSaveInstanceState(savedInstanceState);

		// Save our mobile surface pointer so that we can restore it later
		savedInstanceState.putLong(MOBILE_SURFACE_POINTER_KEY, mSurfaceView.getSurfacePointer());
	}

	@Override
	public void onSurfaceBind(boolean bindRet) {
		// Start load-file asynchronous task if MobileSurface::bind() was successful		
		if (bindRet == false) {
			showToast("C++ bind() failed to initialize");
			return;
		}

		if (mShouldLoadFile) {
			new LoadFileAsyncTask().execute(mPath);
		}
	}

	public void onShowKeyboard() 
	{
		Handler mainHandler = new Handler(Looper.getMainLooper());
		Runnable runnable = new Runnable() {
			@Override
			public void run()
			{
				EditTextWithBackListener hidden_text = (EditTextWithBackListener)mKeyboardTriggerView.findViewById(R.id.hidden_text_field);            
				InputMethodManager inputMethodManager =  (InputMethodManager)getSystemService(INPUT_METHOD_SERVICE);
	            inputMethodManager.toggleSoftInputFromWindow(hidden_text.getApplicationWindowToken(), InputMethodManager.SHOW_FORCED, 0);
	            hidden_text.requestFocus();
			}
		};
		mainHandler.post(runnable);
	}
	
	public void onShowPerformanceTestResult(float fps)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		
	    builder
	    .setTitle("Performance Test")
	    .setMessage("FPS = " +  String.format("%.2f", fps))
	    .setIcon(android.R.drawable.ic_dialog_alert)
	    .setPositiveButton("OK",new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) {
	            
	        }
	    });
	    
	    AlertDialog alert = builder.create();
	    alert.show();
	}
	
	public void eraseKeyboardTriggerField()
	{
		Handler mainHandler = new Handler(Looper.getMainLooper());
		Runnable runnable = new Runnable() {
			@Override
			public void run()
			{
				EditTextWithBackListener hidden_text = (EditTextWithBackListener)mKeyboardTriggerView.findViewById(R.id.hidden_text_field);            
				hidden_text.setText("");
			}
		};
		mainHandler.post(runnable);
	}
	
	// AsyncTask for performing file load on a separate thread.
	// Note that calling cancel() on this task will *not* abort the file load.
	private class LoadFileAsyncTask extends AsyncTask<String, Void[], Boolean> {
		@Override
		protected Boolean doInBackground(String... paths) {
			// Perform file load on separate thread
			return mSurfaceView.loadFile(paths[0]);
		}

		@Override
		protected void onPostExecute(Boolean result) {
			if (result == false)
				showToast("File failed to load");

			if (mProgress != null) {
				mProgress.dismiss();
				mProgress = null;
			}
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mSurfaceView.clearTouches();
	}

	public void finishAll() {
		finish();
	}

	private void showToast(String msg) {
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Show our Operators/Modes/User Code menu options
		// These will get placed on the action bar if they can fit.
		getMenuInflater().inflate(R.menu.toolbars, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle menu option selection

		switch (item.getItemId()) {
		case android.R.id.home:
			// Go back to the SandboxFileList activity
			Intent parentActivityIntent = new Intent(this, SandboxFileListActivity.class);
			parentActivityIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(parentActivityIntent);
			finish();
			return true;
		case R.id.menu_operators:
			// Show operators toolbar
			mMainLayout.removeView(mCurrentToolbarView);
			mCurrentToolbarView = getLayoutInflater().inflate(R.layout.operators, null);
			mMainLayout.addView(mCurrentToolbarView);
			return true;
		case R.id.menu_modes:
			// Show user modes toolbar
			mMainLayout.removeView(mCurrentToolbarView);
			mCurrentToolbarView = getLayoutInflater().inflate(R.layout.modes, null);
			mMainLayout.addView(mCurrentToolbarView);
			return true;
		case R.id.menu_user_code:
			// Show user code toolbar
			mMainLayout.removeView(mCurrentToolbarView);
			mCurrentToolbarView = getLayoutInflater().inflate(R.layout.user_code, null);
			mMainLayout.addView(mCurrentToolbarView);
			return true;
		}

		return super.onOptionsItemSelected(item);
	}


	public void toolbarButtonPressed(View view) {
		// Handle toolbar button press
		// These are connected from xml code via the
		// <ImageButton android:onClick="toolbarButtonPressed"> attribute
		// Calling the method on AndroidUserMobileSurfaceView calls down to the actions
		// in UserMobileSurface.h

		switch (view.getId()) {
		case R.id.orbitButton:
			mSurfaceView.setOperatorOrbit();
			break;
		case R.id.zoomAreaButton:
			mSurfaceView.setOperatorZoomArea();
			break;
		case R.id.selectButton:
			mSurfaceView.setOperatorSelectPoint();
			break;
		case R.id.selectAreaButton:
			mSurfaceView.setOperatorSelectArea();
			break;
		case R.id.flyButton:
			mSurfaceView.setOperatorFly();
			break;
		case R.id.simpleShadowButton:
			mModeSimpleShadowEnabled = !mModeSimpleShadowEnabled;
			mSurfaceView.onModeSimpleShadow(mModeSimpleShadowEnabled);
			break;
		case R.id.smoothButton:
			mSurfaceView.onModeSmooth();
			break;
		case R.id.hiddenLineButton:
			mSurfaceView.onModeHiddenLine();
			break;
		case R.id.frameRateButton:
			mSurfaceView.onModeFrameRate();
			break;
		case R.id.userCode1Button:
			mSurfaceView.onUserCode1();
			break;
		case R.id.userCode2Button:
			mSurfaceView.onUserCode2();
			break;
		case R.id.userCode3Button:
			mSurfaceView.onUserCode3();
			break;
		case R.id.userCode4Button:
			mSurfaceView.onUserCode4();
			break;
		}
	}
}
