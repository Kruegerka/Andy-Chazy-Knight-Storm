package kruegersd2018.sd_iot_app;

import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity implements CompoundButton.OnCheckedChangeListener {

    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef = database.getReference();

    final DatabaseReference BoardLED = myRef.child("BoardLED").child("status");
    final DatabaseReference ESPConection = myRef.child("ESPConection").child("status");


    TextView textView;
    Switch Switch;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textView = findViewById(R.id.TextField);
        Switch = findViewById(R.id.LEDSwitch);
        Switch.setOnCheckedChangeListener(this);

        ESPConection.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                String value = dataSnapshot.getValue(String.class);
                textView.setText("ESP Connection: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked){
        if (isChecked){
            BoardLED.setValue("ON");
        }else{
            BoardLED.setValue("OFF");
        }
    }

}

