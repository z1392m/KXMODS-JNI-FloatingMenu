package com.raacheat.cloneinjector;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.provider.Settings;
import android.content.Intent;
import android.net.Uri;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Switch;
import android.widget.Toast;

/*
 * UI-only floating panel for the companion application.
 * It does not access or modify another process.
 */
public final class KXFloatingMenu {
    private final Activity activity;
    private final WindowManager wm;
    private View panel;
    private WindowManager.LayoutParams params;
    private boolean expanded = true;

    public KXFloatingMenu(Activity activity) {
        this.activity = activity;
        this.wm = (WindowManager) activity.getSystemService(Context.WINDOW_SERVICE);
    }

    public static boolean hasOverlayPermission(Context context) {
        return Build.VERSION.SDK_INT < 23 || Settings.canDrawOverlays(context);
    }

    public static void requestOverlayPermission(Activity activity) {
        if (Build.VERSION.SDK_INT >= 23 && !Settings.canDrawOverlays(activity)) {
            Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                    Uri.parse("package:" + activity.getPackageName()));
            activity.startActivity(intent);
        }
    }

    public void show() {
        if (panel != null) return;
        if (!hasOverlayPermission(activity)) {
            Toast.makeText(activity, "Allow display over other apps, then reopen the menu.",
                    Toast.LENGTH_LONG).show();
            requestOverlayPermission(activity);
            return;
        }

        params = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                Build.VERSION.SDK_INT >= 26
                        ? WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY
                        : WindowManager.LayoutParams.TYPE_PHONE,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
                        | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
                PixelFormat.TRANSLUCENT);
        params.gravity = Gravity.TOP | Gravity.START;
        params.x = 28;
        params.y = 120;

        LinearLayout root = new LinearLayout(activity);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(10), dp(8), dp(10), dp(10));
        GradientDrawable bg = new GradientDrawable();
        bg.setColor(Color.rgb(20, 17, 17));
        bg.setCornerRadius(dp(12));
        bg.setStroke(dp(1), Color.rgb(220, 45, 45));
        root.setBackground(bg);

        LinearLayout header = new LinearLayout(activity);
        header.setGravity(Gravity.CENTER_VERTICAL);
        TextView title = label("KXMODS MATRIX", 15, true);
        header.addView(title, new LinearLayout.LayoutParams(0, dp(40), 1));
        TextView collapse = label("−", 24, true);
        collapse.setGravity(Gravity.CENTER);
        header.addView(collapse, new LinearLayout.LayoutParams(dp(42), dp(40)));
        root.addView(header);

        TextView hint = label("ROS LEGACY  •  v1.1", 10, false);
        hint.setTextColor(Color.LTGRAY);
        root.addView(hint);

        LinearLayout features = new LinearLayout(activity);
        features.setOrientation(LinearLayout.VERTICAL);
        addToggle(features, "Enable Barrett", 0);
        addToggle(features, "Enable RPG", 1);
        addToggle(features, "Enable Speed", 2);
        root.addView(features);

        collapse.setOnClickListener(v -> {
            expanded = !expanded;
            features.setVisibility(expanded ? View.VISIBLE : View.GONE);
            hint.setVisibility(expanded ? View.VISIBLE : View.GONE);
            collapse.setText(expanded ? "−" : "+");
        });

        // Drag using the title/header area.
        header.setOnTouchListener(new View.OnTouchListener() {
            private int startX, startY;
            private float downX, downY;
            @Override public boolean onTouch(View v, MotionEvent e) {
                if (e.getAction() == MotionEvent.ACTION_DOWN) {
                    startX = params.x; startY = params.y;
                    downX = e.getRawX(); downY = e.getRawY();
                    return true;
                }
                if (e.getAction() == MotionEvent.ACTION_MOVE) {
                    params.x = startX + (int)(e.getRawX() - downX);
                    params.y = startY + (int)(e.getRawY() - downY);
                    wm.updateViewLayout(panel, params);
                    return true;
                }
                return e.getAction() == MotionEvent.ACTION_UP;
            }
        });

        panel = root;
        wm.addView(panel, params);
    }

    private void addToggle(LinearLayout parent, String text, int id) {
        LinearLayout row = new LinearLayout(activity);
        row.setGravity(Gravity.CENTER_VERTICAL);
        TextView name = label(text, 13, false);
        row.addView(name, new LinearLayout.LayoutParams(0, dp(42), 1));
        Switch toggle = new Switch(activity);
        toggle.setChecked(NativeMenu.nativeGetFeature(id));
        toggle.setOnCheckedChangeListener((button, checked) ->
                NativeMenu.nativeSetFeature(id, checked));
        row.addView(toggle);
        parent.addView(row);
    }

    private TextView label(String text, int size, boolean bold) {
        TextView view = new TextView(activity);
        view.setText(text);
        view.setTextColor(Color.WHITE);
        view.setTextSize(size);
        if (bold) view.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        return view;
    }

    private int dp(int value) {
        return (int)(value * activity.getResources().getDisplayMetrics().density + 0.5f);
    }

    public void hide() {
        if (panel != null) {
            wm.removeView(panel);
            panel = null;
        }
    }
}
