package kruknightinc.sdtestapp;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;

import java.net.Socket;

public class AsyncTaskActivity extends Activity{

    Socket ESPSocket;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    private class OpenConnection extends AsyncTask<String, Void, String>{

        @Override
        protected  String doInBackground(String... params){
            try{
                ESPSocket = new Socket();
            } catch (Exception IOException){
                System.out.println("IOException Occurred in opening connection");
            }
            return null;
        }
    }
}


