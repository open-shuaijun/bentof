package com.et.bento

import android.Manifest
import android.os.Bundle
import android.os.SystemClock.sleep
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.et.bento.databinding.ActivityMainBinding
import com.et.bentof.Bentof

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

//        if (ContextCompat.checkSelfPermission(
//                this,
//                Manifest.permission.WRITE_EXTERNAL_STORAGE
//            ) != PackageManager.PERMISSION_GRANTED
//        ) {
        ActivityCompat.requestPermissions(
            this,
            arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
            1
        )
//        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method

        Thread {
//            sleep(1000)
//            openFile()
            sleep(5000)
//            stringFromJNI()
            Bentof.getInstance().mp42hls()
//            mp42ts()
        }.start()
    }

    /**
     * A native method that is implemented by the 'bento' native library,
     * which is packaged with this application.
     */

    companion object {
        // Used to load the 'bento' library on application startup.
        init {
            System.loadLibrary("bentof")
        }
    }
}