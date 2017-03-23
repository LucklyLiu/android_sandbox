package com.techsoft3d.hps.sandbox;

import java.io.File;
import java.util.Vector;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * HsfFileAdapter provides SandboxFileListActivity with the layout for each row.
 */
public class HsfFileAdapter extends ArrayAdapter<File> {
	private Context mContext;
	private int mLayoutResourceId;
	private Vector<File> mFiles;
	
	public HsfFileAdapter(Context context, int layoutResourceId, Vector<File> files){
		super(context, layoutResourceId, files);
		
		mContext = context;
		mLayoutResourceId = layoutResourceId;
		mFiles = files;
	}
	
	public View getView(int position, View convertView, ViewGroup parent) {
		View row = convertView;
		
        if (row == null) {
            LayoutInflater inflater = ((Activity)mContext).getLayoutInflater();
            row = inflater.inflate(mLayoutResourceId, parent, false);
        }
        
        setRow(row , position);
        return row;
    }
	
	public void removeFileAtPosition(int position){
		mFiles.remove(position);
		notifyDataSetChanged();
	}
	
	private void setRow(View row, int position){
		File file = mFiles.get(position);
		
		TextView filename = (TextView)row.findViewById(R.id.hsffile_name);
		TextView filesize = (TextView)row.findViewById(R.id.hsffile_size);
		ImageView fileicon = (ImageView)row.findViewById(R.id.hsffile_icon);
		
		filename.setText(file.getName());
		filesize.setText(ViewerUtils.readableFileSize(file.length()));
		
		// Store the file in the view tag.  SandboxFileListActivity will
		// read this when a row is selected.
		row.setTag(file);
		
		setFileIcon(fileicon, file);
	}
	
	private void setFileIcon(ImageView fileicon, File file){
		File previewImage = ViewerUtils.getPreviewImageFile(file);
		
		if (previewImage.exists()) {
			Uri previewURI = Uri.fromFile(previewImage);
			fileicon.setImageURI(previewURI);
		} else {
			fileicon.setImageResource(R.drawable.model_default);
		}
	}
}
