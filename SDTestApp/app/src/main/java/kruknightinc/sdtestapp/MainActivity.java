package kruknightinc.sdtestapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.TextView;



public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    TextView textView;
    WebView webView;
    Button OneSec;
    Button FiveSec;
    Button TenSec;
    Button switchRefresh;
    Button LEDOn;
    Button LEDoff;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LEDOn = findViewById(R.id.LEDOn);
        LEDOn.setOnClickListener(this);

        LEDoff = findViewById(R.id.LEDoff);
        LEDoff.setOnClickListener(this);

        OneSec = findViewById(R.id.OneSec);
        OneSec.setOnClickListener(this);

        FiveSec = findViewById(R.id.FiveSec);
        FiveSec.setOnClickListener(this);

        TenSec = findViewById(R.id.TenSec);
        TenSec.setOnClickListener(this);

        switchRefresh = findViewById(R.id.switchRefresh);
        switchRefresh.setOnClickListener(this);

        textView = findViewById(R.id.textView);
        webView = findViewById(R.id.WebViewer);
    }


    public void onClick(View v){

        switch (v.getId()){
            case R.id.LEDOn:
                webView.loadUrl("http://192.168.4.1/on");
                break;
            case R.id.LEDoff:
                webView.loadUrl("http://192.168.4.1/off");
                break;
            case R.id.OneSec:
                webView.loadUrl("http://192.168.4.1/1sec");
                break;
            case R.id.FiveSec:
                webView.loadUrl("http://192.168.4.1/5sec");
                break;
            case R.id.TenSec:
                webView.loadUrl("http://192.168.4.1/10sec");
                break;
            case R.id.switchRefresh:
                webView.loadUrl("http://192.168.4.1/refresh");
                break;
        }



    }





}
