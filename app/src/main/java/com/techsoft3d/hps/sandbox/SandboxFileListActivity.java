package com.techsoft3d.hps.sandbox;

import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Vector;
import java.util.List;

import android.app.ListActivity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;

/**
 * Initial Android Activity
 */
public class SandboxFileListActivity extends ListActivity {

    static {
        // Load C++ shared libraries
        ViewerUtils.LoadNativeLibs();
    }

    private static boolean mAssetsLoaded = false;

    //加载本地资源
    private static void loadAssets(AssetManager assetManager) {
        if (mAssetsLoaded)
            return;
        // Copy materials from our assets onto sdcard to have a path for HPS to access
        List<String> assets = null;
        try {
            assets = Arrays.asList(assetManager.list(""));
        } catch (IOException e) {
            //e.printStackTrace();
        }

        if (assets != null && assets.contains("datasets")) {
            ViewerUtils.copyAsset(assetManager, "datasets", ViewerUtils.MY_DOCUMENTS_PATH, false);
            Log.i("SandboxApp", "datasets copied to sdcard");
        }

        if (assets != null && assets.contains("fonts")) {
            ViewerUtils.copyAsset(assetManager, "fonts", ViewerUtils.MY_DOCUMENTS_PATH, false);
            Log.i("SandboxApp", "fonts copied to sdcard");
        }

        if (assets != null && assets.contains("materials")) {
            ViewerUtils.copyAsset(assetManager, "materials", ViewerUtils.MY_DOCUMENTS_PATH, false);
            Log.i("SandboxApp", "materials copied to sdcard");
        }

        mAssetsLoaded = true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        loadAssets(getAssets());
        MobileApp.setFontDirectory(ViewerUtils.FONT_DIRECTORY_PATH);
        MobileApp.setMaterialsDirectory(ViewerUtils.MATERIAL_DIRECTORY_PATH);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_toolbars, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_user_web:
                startActivity(new Intent(this, WebClientActivity.class));
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onResume() {
        super.onResume();
        fillListView();
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        // The file name to load is stored in the Views tag.
        // This is converted to a Uri, and passed to MobileSurfaceActivity via an Intent
        File f = (File) v.getTag();
        Uri uri = Uri.fromFile(new File(f.getPath()));

        Intent intent = new Intent(this, MobileSurfaceActivity.class);
        intent.setData(uri);
        startActivity(intent);
    }


    private void populateVector(Vector<File> input, File fileDir) {
        if (fileDir.exists() && fileDir.isDirectory()) {
            File[] fileArray = fileDir.listFiles();
            for (int i = 0; i < fileArray.length; i++) {
                if (canOpenFile(fileArray[i]))
                    input.add(fileArray[i]);
            }
        }
    }

    /**
     * Populates list view with files in Samples and My_Documents folder
     */
    private void fillListView() {
        File sample_documents = new File(ViewerUtils.SAMPLE_DOCUMENTS_PATH);
        File my_documents = new File(ViewerUtils.MY_DOCUMENTS_PATH);

        Vector<File> files = new Vector<File>();
        populateVector(files, sample_documents);
        populateVector(files, my_documents);

        if (files.size() > 0)
            setListAdapter(new HsfFileAdapter(this, R.layout.hsffile_row, files));

    }

    private boolean canOpenFile(File file) {
        if (file.isDirectory())
            return false;

        String ext = ViewerUtils.fileExtension(file);
        boolean isNormalFormat = ext.compareToIgnoreCase("hsf") == 0 ||
                ext.compareToIgnoreCase("stl") == 0 ||
                ext.compareToIgnoreCase("obj") == 0;

        if (ViewerUtils.USING_EXCHANGE)
            return isNormalFormat ||
                    ext.compareToIgnoreCase("pdf") == 0 ||
                    ext.compareToIgnoreCase("prc") == 0 ||
                    ext.compareToIgnoreCase("u3d") == 0 ||
                    ext.compareToIgnoreCase("step") == 0 ||
                    ext.compareToIgnoreCase("jt") == 0 ||
                    ext.compareToIgnoreCase("iges") == 0 ||
                    ext.compareToIgnoreCase("ifc") == 0 ||
                    ext.compareToIgnoreCase("ifczip") == 0 ||
                    ext.compareToIgnoreCase("x_b") == 0 ||
                    ext.compareToIgnoreCase("x_t") == 0 ||
                    ext.compareToIgnoreCase("x_mt") == 0 ||
                    ext.compareToIgnoreCase("xmt_txt") == 0;
        else
            return isNormalFormat;
    }
}
