package br.edu.ufsj.dcomp.ocr;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    Bitmap image;
    Button btnProcess;
    ImageView imgView;
    TextView tv;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imgView = (ImageView) findViewById(R.id.imageView);
        image = BitmapFactory.decodeResource(getResources(),R.drawable.cem_cem_um);
        btnProcess = (Button) findViewById(R.id.btn_process);
        imgView.setImageBitmap(image);

        tv = (TextView) findViewById(R.id.tv);


        final Controller controller = new Controller();
        controller.startJniStructures(getAssets());
        btnProcess.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            tv.setText("Processing...");
            controller.createLabels(image,tv);
            //imgView.setImageBitmap(image);

            }
        });

    }


}
