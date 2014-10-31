package com.buwai.androidjnilibrarysample;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
	
	private Button mbtnCopyFileFromAssets;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mbtnCopyFileFromAssets = (Button) findViewById(R.id.btnCopyFileFromAssets);
		mbtnCopyFileFromAssets.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				
			} 
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
}
