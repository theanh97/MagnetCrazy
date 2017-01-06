/****************************************************************************
 * Copyright (c) 2015 Chukong Technologies Inc.
 * <p>
 * http://www.cocos2d-x.org
 * <p>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.Point;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import org.cocos2dx.lib.Cocos2dxActivity;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;

import static com.loopj.android.http.AsyncHttpClient.log;

public class AppActivity extends Cocos2dxActivity {
  private static AppActivity _appActiviy;
  private AdView adView;
  public static DatabaseReference myRef;
  public static ArrayList<Float> arrScore;
  private static final String AD_UNIT_ID = "ca-app-pub-6190597215997421/4975152990";

  public static final String MyPREFERENCES = "MyPrefs";
  private static final String GAME_GUIDE_TAG = "Guide_Tag";
  private static final String GAME_BEST_SCORE_TAG = "Best_Score";

  // Helper get display screen to avoid deprecated function use
  private Point getDisplaySize(Display d) {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
      return getDisplaySizeGE11(d);
    }
    return getDisplaySizeLT11(d);
  }

  @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
  private Point getDisplaySizeGE11(Display d) {
    Point p = new Point(0, 0);
    d.getSize(p);
    return p;
  }

  private Point getDisplaySizeLT11(Display d) {
    try {
      Method getWidth = Display.class.getMethod("getWidth", new Class[]{});
      Method getHeight = Display.class.getMethod("getHeight", new Class[]{});
      return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(), ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
    } catch (NoSuchMethodException e2)  // None of these exceptions should ever occur.
    {
      return new Point(-1, -1);
    } catch (IllegalArgumentException e2) {
      return new Point(-2, -2);
    } catch (IllegalAccessException e2) {
      return new Point(-3, -3);
    } catch (InvocationTargetException e2) {
      return new Point(-4, -4);
    }
  }


  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

    int width = getDisplaySize(getWindowManager().getDefaultDisplay()).x;

    log.d("tag123", "created");
    LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
        width,
        LinearLayout.LayoutParams.WRAP_CONTENT);


    adView = new AdView(this);
    adView.setAdSize(AdSize.BANNER);
    adView.setAdUnitId(AD_UNIT_ID);


    AdRequest adRequest = new AdRequest.Builder()
        .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
        .addTestDevice("C588BB365F058357E7EA4343C7D42C0")
        .build();

    adView.loadAd(adRequest);
    adView.setBackgroundColor(Color.BLACK);
    adView.setBackgroundColor(0);
    addContentView(adView, adParams);

    _appActiviy = this;

    // FIREBASE
    arrScore = new ArrayList<Float>();
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    myRef = database.getReference("top_10");
  }

  public static void createTopTen() {
    arrScore.clear();
    myRef.addValueEventListener(new ValueEventListener() {
      @Override
      public void onDataChange(DataSnapshot dataSnapshot) {
        for (DataSnapshot item : dataSnapshot.getChildren()) {
          arrScore.add(item.getValue(Float.class));
          Log.d("tag456", "create : " + item.getValue(Float.class));
        }
      }

      @Override
      public void onCancelled(DatabaseError databaseError) {
        Log.d("tag456", "fail");
      }
    });
  }

  public static float[] getTopTen() {
    float[] a = new float[]{0,0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 10; i++) {
      a[i] = (float) arrScore.get(i);
      Log.d("tag456", "update : " + a[i]);
    }
    return a;
  }
  public static boolean isConnection(){
    Log.d("tag456","call isConnection") ;
      ConnectivityManager cm = (ConnectivityManager) _appActiviy.getSystemService(Context
          .CONNECTIVITY_SERVICE);
    Log.d("tag456","isConnection : " + cm.getActiveNetworkInfo());
    return cm.getActiveNetworkInfo() != null;
  }
  public static void updateTopTen(float newBestScore) {
    Log.d("tag32","size : " + arrScore.size());
    for (int i = 0; i < 10; i++) {
      if (arrScore.get(i) < newBestScore) {
        for (int j = 8; j >= i; j--) {
          arrScore.set(j + 1, arrScore.get(j));
        }
        arrScore.set(i, newBestScore);
        updateFireBase();
        return;
      }
    }
  }

  public static void updateFireBase() {
    for (int i = 1; i <= 10; i++) {
      String number = i + "";
      myRef.child(number).setValue(arrScore.get(i - 1));
    }
  }

  public static void hideAd() {
    _appActiviy.runOnUiThread(new Runnable() {

      @Override
      public void run() {
        if (_appActiviy.adView.isEnabled())
          _appActiviy.adView.setEnabled(false);
        if (_appActiviy.adView.getVisibility() != 4)
          _appActiviy.adView.setVisibility(View.INVISIBLE);
      }
    });

  }

  public static void showAd() {
    _appActiviy.runOnUiThread(new Runnable() {

      @Override
      public void run() {
        if (!_appActiviy.adView.isEnabled())
          _appActiviy.adView.setEnabled(true);
        if (_appActiviy.adView.getVisibility() == 4)
          _appActiviy.adView.setVisibility(View.VISIBLE);
      }
    });

  }

  @Override
  protected void onResume() {
    super.onResume();
    if (adView != null) {
      adView.resume();
    }
  }

  @Override
  protected void onPause() {
    if (adView != null) {
      adView.pause();
    }
    super.onPause();
  }

  @Override
  protected void onDestroy() {
    adView.destroy();
    super.onDestroy();
  }

  public static short getBestScore() {
    SharedPreferences preferences = _appActiviy.getSharedPreferences(MyPREFERENCES, MODE_PRIVATE);
    SharedPreferences.Editor editor = preferences.edit();

    int bestSCore = preferences.getInt(GAME_BEST_SCORE_TAG, 0);
    Log.d("tag123", "Best Score : " + bestSCore);
    if (bestSCore == 0) {
      editor.putInt(GAME_BEST_SCORE_TAG, 0);
      editor.commit();
    }
    return (short) bestSCore;
  }

  public static void updateBestScore(short bestScore) {
    SharedPreferences preferences = _appActiviy.getSharedPreferences(MyPREFERENCES, MODE_PRIVATE);
    SharedPreferences.Editor editor = preferences.edit();
    editor.putInt(GAME_BEST_SCORE_TAG, (int) bestScore);
    editor.commit();
  }

  public static short getGameGuideFlag() {
    SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(_appActiviy);
    SharedPreferences.Editor editor = preferences.edit();

    int result = preferences.getInt(GAME_GUIDE_TAG, 0) + 1;
    Log.d("tag123", "Game Guide : " + (short) result);
    if (result == 1)
      editor.putInt(GAME_GUIDE_TAG, 1);
    else {
      result = result + 5;
      editor.putInt(GAME_GUIDE_TAG, result + 5);
    }
    editor.apply();
    return (short) result;
  }
}
