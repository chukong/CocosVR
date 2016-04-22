package org.cocos2dx.cpp;

import android.content.Context;

import com.google.vrtoolkit.cardboard.CardboardDeviceParams;
import com.google.vrtoolkit.cardboard.Distortion;
import com.google.vrtoolkit.cardboard.Eye;
import com.google.vrtoolkit.cardboard.FieldOfView;
import com.google.vrtoolkit.cardboard.HeadMountedDisplayManager;
import com.google.vrtoolkit.cardboard.ScreenParams;
import com.google.vrtoolkit.cardboard.sensors.HeadTracker;
import com.google.vrtoolkit.cardboard.DistortionRenderer;

public class CbAPIWrapper {
    private static  HeadMountedDisplayManager mHmdManager;
    private static  HeadTracker               mHeadTracker;
    private static  DistortionRenderer        mDistortionRenderer;
    private static  final  Eye[]              mEyes = new Eye[2];
    private static  boolean                   mIsEnterVRMode = false;

    public static void initialize(Context context, float[] params){
        mHeadTracker        = HeadTracker.createFromContext(context);
        mHmdManager         = new HeadMountedDisplayManager(context);
        mDistortionRenderer = new DistortionRenderer();
        mEyes[0]            = new Eye(Eye.Type.LEFT);
        mEyes[1]            = new Eye(Eye.Type.RIGHT);

        CardboardDeviceParams cdp = mHmdManager.getHeadMountedDisplay().getCardboardDeviceParams();
        ScreenParams sp = mHmdManager.getHeadMountedDisplay().getScreenParams();
        params[0] = sp.getWidth();
        params[1] = sp.getHeight();
        params[2] = sp.getWidthMeters();
        params[3] = sp.getHeightMeters();
        params[4] = sp.getBorderSizeMeters();

        params[5] = cdp.getInterLensDistance();
        if (cdp.getVerticalAlignment() == CardboardDeviceParams.VerticalAlignmentType.BOTTOM){
            params[6] = 0;
        }else if (cdp.getVerticalAlignment() == CardboardDeviceParams.VerticalAlignmentType.CENTER){
            params[6] = 1;
        }else{
            params[6] = 2;
        }

        params[7] = cdp.getVerticalDistanceToLensCenter();
        params[8] = cdp.getScreenToLensDistance();

        params[9] = cdp.getLeftEyeMaxFov().getLeft();
        params[10] = cdp.getLeftEyeMaxFov().getRight();
        params[11] = cdp.getLeftEyeMaxFov().getBottom();
        params[12] = cdp.getLeftEyeMaxFov().getTop();
    }

    public static void getEyes(float[] eyesParams){
        CardboardDeviceParams cdp = mHmdManager.getHeadMountedDisplay().getCardboardDeviceParams();
        float eyeToScreenDis = updateFieldOfView(mEyes[0].getFov(), mEyes[1].getFov());
        mDistortionRenderer.onFovChanged(mHmdManager.getHeadMountedDisplay(), mEyes[0].getFov(), mEyes[1].getFov(), eyeToScreenDis);
        mDistortionRenderer.updateViewports(mEyes[0].getViewport(), mEyes[1].getViewport());

        //viewport
        eyesParams[0] = mEyes[0].getViewport().x;
        eyesParams[1] = mEyes[0].getViewport().y;
        eyesParams[2] = mEyes[0].getViewport().width;
        eyesParams[3] = mEyes[0].getViewport().height;
        //fov
        eyesParams[4] = mEyes[0].getFov().getLeft();
        eyesParams[5] = mEyes[0].getFov().getRight();
        eyesParams[6] = mEyes[0].getFov().getBottom();
        eyesParams[7] = mEyes[0].getFov().getTop();

        //viewport
        eyesParams[8] = mEyes[1].getViewport().x;
        eyesParams[9] = mEyes[1].getViewport().y;
        eyesParams[10] = mEyes[1].getViewport().width;
        eyesParams[11] = mEyes[1].getViewport().height;
        //fov
        eyesParams[12] = mEyes[1].getFov().getLeft();
        eyesParams[13] = mEyes[1].getFov().getRight();
        eyesParams[14] = mEyes[1].getFov().getBottom();
        eyesParams[15] = mEyes[1].getFov().getTop();
    }

    public static void enterVrMode(){
        mHeadTracker.startTracking();
        mIsEnterVRMode = true;
    }

    public static void leaveVrMode(){
        mHeadTracker.stopTracking();
        mIsEnterVRMode = false;
    }

    public static void getLastHeadView(float[] viewMat){
        mHeadTracker.getLastHeadView(viewMat, 0);
    }

    public static void beforeDrawFrame(){
        if (!mIsEnterVRMode) return;
        mDistortionRenderer.beforeDrawFrame();
    }

    public static void afterDrawFrame(){
        if (!mIsEnterVRMode) return;
        mDistortionRenderer.afterDrawFrame();
    }

    private static float updateFieldOfView(FieldOfView leftEyeFov, FieldOfView rightEyeFov) {
        CardboardDeviceParams cdp = mHmdManager.getHeadMountedDisplay().getCardboardDeviceParams();
        ScreenParams screen = mHmdManager.getHeadMountedDisplay().getScreenParams();
        Distortion distortion = cdp.getDistortion();

        //float idealFovAngle = (float)Math.toDegrees(Math.atan2(cdp.getLensDiameter() / 2.0F, cdp.getEyeToLensDistance()));

        float eyeToScreenDist = 0.011F + cdp.getScreenToLensDistance();

        float outerDist = (screen.getWidthMeters() - cdp.getInterLensDistance()) / 2.0F;

        float innerDist = cdp.getInterLensDistance() / 2.0F;
        float bottomDist = cdp.getVerticalDistanceToLensCenter() - screen.getBorderSizeMeters();

        float topDist = screen.getHeightMeters() + screen.getBorderSizeMeters() - cdp.getVerticalDistanceToLensCenter();

        float outerAngle = (float)Math.toDegrees(Math.atan2(distortion.distort(outerDist), eyeToScreenDist));

        float innerAngle = (float)Math.toDegrees(Math.atan2(distortion.distort(innerDist), eyeToScreenDist));

        float bottomAngle = (float)Math.toDegrees(Math.atan2(distortion.distort(bottomDist), eyeToScreenDist));

        float topAngle = (float)Math.toDegrees(Math.atan2(distortion.distort(topDist), eyeToScreenDist));

        leftEyeFov.setLeft(Math.min(outerAngle, leftEyeFov.getLeft()));
        leftEyeFov.setRight(Math.min(innerAngle, leftEyeFov.getRight()));
        leftEyeFov.setBottom(Math.min(bottomAngle, leftEyeFov.getBottom()));
        leftEyeFov.setTop(Math.min(topAngle, leftEyeFov.getTop()));

        rightEyeFov.setLeft(Math.min(innerAngle, rightEyeFov.getLeft()));
        rightEyeFov.setRight(Math.min(outerAngle, rightEyeFov.getRight()));
        rightEyeFov.setBottom(Math.min(bottomAngle, rightEyeFov.getBottom()));
        rightEyeFov.setTop(Math.min(topAngle, rightEyeFov.getTop()));

        return eyeToScreenDist;
    }
}
