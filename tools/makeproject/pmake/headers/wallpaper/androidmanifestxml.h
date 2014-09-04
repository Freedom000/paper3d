const char ANDROIDMANIFESTXML_STR[] = 
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
"    package=\"com.fi.tech.%s\"\n"
"    android:versionCode=\"1\"\n"
"    android:versionName=\"%s\" >\n"
"\n"
"    <uses-permission android:name=\"android.permission.WAKE_LOCK\" />\n"
"    <uses-permission android:name=\"android.permission.MOUNT_UNMOUNT_FILESYSTEMS\" />\n"
"    <uses-permission android:name=\"android.permission.SET_WALLPAPER\" />\n"
"    <uses-permission android:name=\"android.permission.READ_EXTERNAL_STORAGE\" />\n"
"\n"
"    <uses-sdk\n"
"        android:minSdkVersion=\"9\"\n"
"        android:targetSdkVersion=\"9\" />\n"
"\n"
"    <application\n"
"        android:debuggable=\"true\"\n"
"        android:icon=\"@drawable/icon\"\n"
"        android:label=\"@string/app_name\" >\n"
"        <service\n"
"            android:name=\".Paper3D\"\n"
"            android:configChanges=\"keyboardHidden|navigation|orientation\"\n"
"            android:enabled=\"true\"\n"
"            android:label=\"@string/service_label\"\n"
"            android:permission=\"android.permission.BIND_WALLPAPER\" >\n"
"            <intent-filter>\n"
"                <action android:name=\"android.service.wallpaper.WallpaperService\" />\n"
"            </intent-filter>\n"
"\n"
"            <meta-data\n"
"                android:name=\"android.service.wallpaper\"\n"
"                android:resource=\"@xml/wallpaper_settings\" />\n"
"        </service>\n"
"\n"
"        <activity\n"
"            android:name=\".Paper3DSettings\"\n"
"            android:exported=\"true\"\n"
"            android:label=\"@string/settings\"\n"
"            android:launchMode=\"singleTask\"\n"
"            android:noHistory=\"true\"\n"
"            android:theme=\"@android:style/Theme.Light.WallpaperSettings\"\n"
"            android:windowSoftInputMode=\"stateUnchanged\" >\n"
"            <intent-filter>\n"
"                <action android:name=\"android.intent.action.MAIN\" />\n"
"            </intent-filter>\n"
"        </activity>\n"
"\n"
"    </application>\n"
"\n"
"</manifest>\n"
;