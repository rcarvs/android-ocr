package br.edu.ufsj.dcomp.ocr;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;


public class MainActivity extends AppCompatActivity {

    Bitmap image;
    Button btnProcess;
    ImageView imgView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imgView = (ImageView) findViewById(R.id.imageView);
        image = BitmapFactory.decodeResource(getResources(),R.drawable.contra_exemplo_crossing);
        btnProcess = (Button) findViewById(R.id.btn_process);
        imgView.setImageBitmap(image);
        final Controller controller = new Controller();

        btnProcess.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                controller.createLabels(image);
                imgView.setImageBitmap(image);
            }
        });

    }


}
