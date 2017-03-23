package com.techsoft3d.hps.sandbox;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.DecimalFormat;

import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

public class ViewerUtils {
	/**
	 * SD卡目录文件绝对地址
	 */
	public final static String MY_DOCUMENTS_PATH = Environment.getExternalStorageDirectory() + "/HPSSandbox";//存储路径
	public final static String SAMPLE_DOCUMENTS_PATH = MY_DOCUMENTS_PATH + "/datasets";//hfs显示路径
	public final static String FONT_DIRECTORY_PATH = MY_DOCUMENTS_PATH + "/fonts";//字体样式
	public final static String MATERIAL_DIRECTORY_PATH = MY_DOCUMENTS_PATH + "/materials";
	public final static String EXTRA_DIR_VIEW_TYPE = "com.techsoft.hoopsviewer.VIEW_TYPE";
	public final static boolean USING_EXCHANGE = false;
	public final static int DIRECTORY_VIEW_TYPE = 1;
	private static boolean mNativeLibsLoaded = false;

	//
	public static String fileExtension(File file) {
		String[] parts = file.getName().split("\\.");
		return parts[parts.length - 1];
	}

	//加载本地Libs库文件
	public static void LoadNativeLibs(){
		if (!mNativeLibsLoaded){
			System.loadLibrary("gnustl_shared");
			if (USING_EXCHANGE)
			System.loadLibrary("A3DLIBS");
			System.loadLibrary("hps_core");
			System.loadLibrary("hps_sprk");
			System.loadLibrary("hps_sprk_ops");
			if (USING_EXCHANGE)
			System.loadLibrary("hps_sprk_exchange");
			System.loadLibrary("android_sandbox");
			mNativeLibsLoaded = true;
		}
	}
	//判断当前文件的位置以及是否重复
	public static boolean filenameInDirectory(String filename, String directory){
		File dir = new File(directory);
		if (!dir.exists() || !dir.isDirectory())
			return false;
		
		String[] files = dir.list();
		
		for (int i = 0; i < files.length; i ++){
			if (files[i].compareToIgnoreCase(filename) == 0)
				return true;
		}
		
		return false;
	}
	
	//读取文件的大小
	public static String readableFileSize(long size) {
	    if (size <= 0) return "0";
	    final String[] units = new String[] { "B", "KB", "MB", "GB", "TB" };
	    int digitGroups = (int) (Math.log10(size)/Math.log10(1024));
	    return new DecimalFormat("#,##0.#").format(size/Math.pow(1024, digitGroups)) + " " + units[digitGroups];
	}
	//获取图片地址
	public static File getPreviewImageFile(File file) {
		String fullpath = file.getPath();
		String previewPath = fullpath.substring(0, fullpath.lastIndexOf('.'));
		previewPath = previewPath.concat(".png");
		return new File(previewPath);
	}

	public static boolean CopyFile(String src, String dst){
		try{
			FileInputStream in = new FileInputStream(src);
			return CopyFile(in, dst);
		}
		catch (IOException e) {
			return false;
		}
	}
	
	public static boolean CopyFile(InputStream in, String dest){
		try{
			FileOutputStream out = new FileOutputStream(dest);
	
		    // Transfer bytes from in to out
		    byte[] buf = new byte[1024];
		    int len;
		    while ((len = in.read(buf)) > 0) {
		        out.write(buf, 0, len);
		    }
		    
		    in.close();
		    out.close();
		}
		catch (IOException e) {
			return false;
		}
		
		return true;
	}
	
	/**将文件从资产管理器复制到外部存储目录
	 * Copies file from asset manager to external storage directory
	 * @param assetManager	AssetManager containing resources
	 * @param fileName	Relative path to file in asset manager
	 * @param targetDirName	External storage directory name
	 * @param overwriteExisting	Pass true to overwrite existing file
	 */
	public static void copyAssetFile(AssetManager assetManager, String fileName, String targetDirName, boolean overwriteExisting) {
		File file = new File(targetDirName, fileName);
		if (!overwriteExisting && file.exists())
			return;
		
		try {
			InputStream istream = assetManager.open(fileName);
			OutputStream ostream = new BufferedOutputStream(new FileOutputStream(file));
			
			byte[] buffer = new byte[1024];
			int len = istream.read(buffer);
			while (len != -1) {
				ostream.write(buffer, 0, len);
				len = istream.read(buffer);
			}
			ostream.close();
			istream.close();
		}
		catch (FileNotFoundException e) {
			Log.e("ViewerUtils", "File not found in asset manager: " + fileName, e);
		}
		catch (IOException e) {
			Log.e("ViewerUtils", "IOException", e);
		}
	}
	
	/**将文件或目录从资产管理器复制到外部存储目录
	 * Copies file or directory from asset manager to external storage directory.
	 * @param assetManager	AssetManager containing resources
	 * @param path	Relative path to file or directory in asset manager
	 * @param targetDirName	External storage directory name	
	 * @param overwriteExisting	Pass true to overwrite existing file
	 */
	public static void copyAsset(AssetManager assetManager, String path, String targetDirName, boolean overwriteExisting) {
		try {
			// If path is a file, assets will be null or 0 length.
			String[] assets = assetManager.list(path);
			
			if (assets == null || assets.length == 0) {
				copyAssetFile(assetManager, path, targetDirName, overwriteExisting);
			} else {
				File targetDir = new File(targetDirName, path);
				targetDir.mkdirs();
				
				for (String subDirName : assets) {
					copyAsset(assetManager, path + "/" + subDirName, targetDirName, overwriteExisting);
				}
			}
			
		}
		catch (IOException e) {
			Log.e("ViewerUtils", "IOException", e);
		}	
	}
}
