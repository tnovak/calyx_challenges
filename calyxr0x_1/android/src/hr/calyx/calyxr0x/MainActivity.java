package hr.calyx.calyxr0x;

import java.math.BigInteger;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity implements TextWatcher {

	private BigInteger mModulus;
	private BigInteger mExponent;
	private BigInteger mTarget;
	
	private EditText mInputText;
	private TextView mEncryptedView;
	private TextView mStatusView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mModulus = new BigInteger(getString(R.string.modulus), 16);
		mExponent = new BigInteger(getString(R.string.exponent), 16);
		mTarget = new BigInteger(getString(R.string.target), 16);
		
		mInputText = (EditText)findViewById(R.id.txt_input);
		mEncryptedView = (TextView)findViewById(R.id.txt_encrypted);
		mStatusView = (TextView)findViewById(R.id.txt_status);

		mInputText.addTextChangedListener(this);
	}
	
	@Override
	public void afterTextChanged(Editable s) {
		BigInteger x;
		
		try {
			x = new BigInteger(mInputText.getText().toString(), 10);
		} catch (NumberFormatException e) {
			mStatusView.setText("");
			mEncryptedView.setText("");
			return;
		}
		
		x = encrypt(x);
		byte[] value = x.toByteArray();
		
		StringBuilder sb = new StringBuilder();
		for (byte b : value) {			
			if (Character.isLetterOrDigit(b)) {
				sb.append((char)b);
			} else {
				sb.append("0x");
				String hex = Integer.toHexString(((int)b) & 0xFF);
				if (hex.length() < 2) {
					sb.append("0");
				}
				sb.append(hex);
			}
			
			sb.append(" ");
		}
		mEncryptedView.setText(sb.toString());
		
		if (mTarget.equals(x)) {
			mStatusView.setText(R.string.txt_correct);
			mStatusView.setTextColor(getResources().getColor(R.color.correct));
		} else {
			mStatusView.setText(R.string.txt_incorrect);
			mStatusView.setTextColor(getResources().getColor(R.color.incorrect));
		}
	}
	
	@Override
	public void beforeTextChanged(CharSequence s, int start, int count,
			int after) {}
	
	@Override
	public void onTextChanged(CharSequence s, int start, int before, int count) {}
	
	private BigInteger encrypt(BigInteger input) {
		return input.modPow(mExponent, mModulus);
	}
}
