package com.techsoft3d.hps.sandbox;

import android.content.Context;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.EditText;

public class EditTextWithBackListener extends EditText {

	public EditTextWithBackListener(Context context, AttributeSet attrs,
			int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
	}

	public EditTextWithBackListener(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public EditTextWithBackListener(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	public boolean onKeyPreIme(int keyCode, KeyEvent event) {
	    if (keyCode == KeyEvent.KEYCODE_BACK && 
	        event.getAction() == KeyEvent.ACTION_UP) {
	    		mActivity.eraseKeyboardTriggerField();
	    		mSurfaceView.onKeyboardHidden();
	            return false;
	    }
	    return super.dispatchKeyEvent(event);
	}
	
	public void setSurfaceActivity(MobileSurfaceActivity activity)
	{
		mActivity = activity;
	}
	
	public void setSurfaceView(AndroidUserMobileSurfaceView surfaceView)
	{
		mSurfaceView = surfaceView;
	}

	private MobileSurfaceActivity mActivity;
	private AndroidUserMobileSurfaceView mSurfaceView;
}
