package com.et.bento

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.SystemClock.sleep
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.et.bento.databinding.ActivityMainBinding
import com.et.bentof.Bentof
import java.io.File

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private val directory: File? by lazy {
        getExternalFilesDir("movie")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                1
            )
        } else {
            Log.d("TAG", "permission ok")
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)


        Thread {
            directory ?: return@Thread
            val assetsName = "h5.h264"
            sleep(500)
            // 准备视频文件
            FileUtils.assetsCopyTo(applicationContext, assetsName, directory!!)
            sleep(500)
            // H.264封装MP4
            val muxResult = Bentof.getInstance().mp4mux(
                File(directory, assetsName).absolutePath,
                File(directory, "h5_mux.mp4").absolutePath
            )
            Log.d("TAG", "MuxResult=$muxResult")
            sleep(500)
            val hlsResult = Bentof.getInstance().mp42hls(
                File(directory, "h5_mux.mp4").absolutePath,
                directory!!.absolutePath + File.separator,
                "h5_hls.m3u8",
                "h5_ts_%05d.ts"
            )
            Log.d("TAG", "HlsResult=$hlsResult")

            sleep(500)
            val tsResult = Bentof.getInstance().mp42ts(File(directory, "h5_mux.mp4").absolutePath, File(directory,"h5_mux_ts.ts").absolutePath);
            Log.d("TAG", "TSResult=$tsResult")


        }.start()
    }


}