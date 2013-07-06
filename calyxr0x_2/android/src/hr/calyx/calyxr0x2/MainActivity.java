package hr.calyx.calyxr0x2;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.res.TypedArray;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

public class MainActivity extends Activity {

	private static final String KEY_BUTTONS = "hr.calyx.calyxr0x2.buttons";
	private static final String KEY_INPUT   = "hr.calyx.calyxr0x2.input";
	private static final int    NUM_BUTTONS = 64;
	
	private String mInput;
	private boolean[] mButtons;
	private List<List<Integer>> mGraph;
	
	private TextView mInputSet;
	private List<ToggleButton> mButtonViews;
	private GridView mButtonGrid;
	
	private Random mRandom = new Random();
	
	@Override
	protected void onCreate(Bundle args) {
		super.onCreate(args);
		setContentView(R.layout.activity_main);

		if (args != null && args.containsKey(KEY_BUTTONS) && 
				args.containsKey(KEY_INPUT)) {
			
			mInput = args.getString(KEY_INPUT);
			mButtons = args.getBooleanArray(KEY_BUTTONS);
		} else {
			selectInput();
			mButtons = new boolean[NUM_BUTTONS];
		}

		initGraph(mInput);
		initUI();
		
		Toast.makeText(this, R.string.description, Toast.LENGTH_LONG).show();
	}
	
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		
		outState.putString(KEY_INPUT, mInput);
		outState.putBooleanArray(KEY_BUTTONS, mButtons);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		if (item.getItemId() == R.id.menu_reset) {
			reset();

			return true;
		} else if (item.getItemId() == R.id.menu_new) {
			reset();
			selectInput();
			initGraph(mInput);
			showInputSet();
			
			return true;
		} else {
			return super.onOptionsItemSelected(item);
		}
	}
	
	private void reset() {
		mButtons = new boolean[NUM_BUTTONS];
		for (ToggleButton b : mButtonViews) {
			b.setChecked(false);
		}
	}
	
	private void selectInput() {
		TypedArray a = getResources().obtainTypedArray(R.array.inputs);
		mInput = a.getString(mRandom.nextInt(NUM_BUTTONS));
		a.recycle();
	}
	
	private void initUI() {
		mInputSet = (TextView)findViewById(R.id.input_set);
		showInputSet();
		
		LayoutInflater inflater = LayoutInflater.from(this);
		
		mButtonViews = new ArrayList<ToggleButton>();
		for (int i = 0; i < NUM_BUTTONS; ++i) {
			ToggleButton button = (ToggleButton)inflater.inflate(R.layout.button, null);
			String label = "" + (i+1);
			
			button.setText(label);
			button.setTextOn(label);
			button.setTextOff(label);

			button.setOnClickListener(new ButtonClickListener(i));
			mButtonViews.add(button);
		}
		
		mButtonGrid = (GridView)findViewById(R.id.button_grid);
		mButtonGrid.setAdapter(new ButtonAdapter());
		
		for (int i = 0; i < NUM_BUTTONS; ++i) {
			if (mButtons[i]) {
				mButtons[i] = false;
				mButtonViews.get(i).performClick();
			}
		}		
	}
	
	private void showInputSet() {
		mInputSet.setText(getString(R.string.input_set, mInput.substring(0, 8) + "..."));
	}
	
	private void initGraph(String s) {
		if ((s.length() & 1) != 0) {
			throw new IllegalStateException("Input length invalid");
		}
		
		mGraph = new ArrayList<List<Integer>>();
		for (int i = 0; i < NUM_BUTTONS; ++i) {
			mGraph.add(new ArrayList<Integer>());
		}
		
		int[] next = new int[2*NUM_BUTTONS];
		Arrays.fill(next, -1);
		
		int cur = 0, p = 0, j = 0;
		for (int i = 0; i < s.length(); i += 2) {
			int x = Integer.parseInt(s.substring(i, i+2), 16);
			
			cur |= (x << p); p += 8;
			while (p >= 6) {
				int y = cur & 63; cur >>= 6; p -= 6;
				next[j++] = y;
			}
		}
		while (p >= 6) {
			int y = cur & 63; cur >>= 6; p -= 6;
			next[j++] = y;
		}
		
		if (j != 2*NUM_BUTTONS) {
			throw new IllegalStateException("Invalid input");
		}
		
		for (int i = 0; i < NUM_BUTTONS; ++i) {
			for (int k = 0; k < 2; ++k) {
				mGraph.get(next[2*i + k]).add(i);
			}
		}
	}
	
	private boolean isDone() {
		for (int i = 0; i < NUM_BUTTONS; ++i) {
			if (!mButtonViews.get(i).isChecked()) {
				return false;
			}
		}
		
		return true;
	}
	
	private void checkState() {
		if (isDone()) {
			BigInteger buttonState = BigInteger.ZERO;
			for (int i = 0; i < NUM_BUTTONS; ++i) {
				if (mButtons[i]) {
					buttonState = buttonState.setBit(i);
				}
			}
			
			new AlertDialog.Builder(this)
				.setTitle(R.string.title)
				.setMessage(getString(R.string.message, mInput.substring(0, 8), buttonState.toString(16)))
				.setPositiveButton(android.R.string.ok, null)
				.show();
		}
	}
	
	class ButtonClickListener implements View.OnClickListener {
		int mPos;
		
		public ButtonClickListener(int pos) {
			mPos = pos;
		}
		
		@Override
		public void onClick(View v) {
			mButtonViews.get(mPos).toggle();
			
			mButtons[mPos] ^= true;
			for (Integer i : mGraph.get(mPos)) {
				mButtonViews.get(i).toggle();
			}
			
			checkState();
		}
	}
	
	class ButtonAdapter extends BaseAdapter {

		@Override
		public int getCount() {
			return NUM_BUTTONS;
		}

		@Override
		public Object getItem(int position) {
			return null;
		}

		@Override
		public long getItemId(int position) {
			return position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			return mButtonViews.get(position);
		}
	}
}
