/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014-2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logger.h"
#include "npc_animator.h"

static  QPixmap *tempImage = nullptr;
static  int AdvNpcAnimator_count = 0;

inline void constructTempImage()
{
    if(!tempImage)
    {
        tempImage = new QPixmap(QSize(32, 32));
        tempImage->fill(QColor(Qt::red));
    }

    AdvNpcAnimator_count++;
}

inline void destroyTempImage()
{
    AdvNpcAnimator_count--;

    if((AdvNpcAnimator_count <= 0) && (tempImage))
    {
        delete tempImage;
        tempImage = nullptr;
    }
}

AdvNpcAnimator::AdvNpcAnimator():
    timer(nullptr)
{
    constructTempImage();
}

AdvNpcAnimator::AdvNpcAnimator(QPixmap &sprite, obj_npc &config):
    timer(nullptr)
{
    constructTempImage();
    buildAnimator(sprite, config);
}

AdvNpcAnimator::~AdvNpcAnimator()
{
    if(timer)
        delete timer;

    destroyTempImage();
}

void AdvNpcAnimator::buildAnimator(QPixmap &sprite, obj_npc &config)
{
    if(timer)
    {
        timer->stop();
        delete timer;
    }

    //frames.clear();
    mainImage = &sprite;
    setup = config;
    animated = false;
    aniBiDirect = false;
    curDirect = -1;
    frameStep = 1;
    frameHeight = 1;
    CurrentFrameL = 0; //Real frame
    CurrentFrameR = 0; //Real frame
    frameCurrentL = 0; //Timer frame
    frameCurrentR = 0; //Timer frame
    frameFirstL =  0; //from first frame
    frameLastL  = -1; //to unlimited frameset
    frameFirstR =  0; //from first frame
    frameLastR  = -1; //to unlimited frameset
    animated = true;
    frameSpeed = static_cast<int>(setup.setup.framespeed);
    frameStyle = (int)setup.setup.framestyle;
    frameStep  = 1;
    frameSequance = false;
    aniBiDirect  = setup.setup.ani_bidir;
    customAniAlg = setup.setup.custom_ani_alg;
    customAnimate = setup.setup.custom_animate;
    custom_frameFL = setup.setup.custom_ani_fl;//first left
    custom_frameEL = setup.setup.custom_ani_el;//end left
    custom_frameFR = setup.setup.custom_ani_fr;//first right
    custom_frameER = setup.setup.custom_ani_er;//enf right
    //bool refreshFrames = updFrames;
    frameHeight = (int)setup.setup.gfx_h; // height of one frame
    frameWidth  = (int)setup.setup.gfx_w; //width of target image
    spriteHeight = mainImage->height(); // Height of target image
    framesCountOneSide = static_cast<int>(setup.setup.frames);
    //int framesMod = spriteHeight % frameHeight;
    //int roundedSpriteHeight = (spriteHeight-framesMod) / frameHeight;
    int framesZoneHeight = 0;
    framesCountTotal = 0;

    //Protectors
    if(frameHeight <= 0) frameHeight = 1;
    if(frameHeight > spriteHeight) frameHeight = spriteHeight;
    if(frameWidth <= 0) frameWidth = 1;

    if(frameWidth > mainImage->width())
        frameWidth = mainImage->width();

    int frameBottomSpace = spriteHeight % frameHeight;
    framesZoneHeight = spriteHeight + (frameBottomSpace == 0 ? 0 : frameHeight - frameBottomSpace);
    framesCountTotal = framesZoneHeight / frameHeight;

    //while(framesZoneHeight < spriteHeight)
    //{
    //    framesCountTotal++;
    //    framesZoneHeight += frameHeight;
    //}

    if(setup.setup.ani_directed_direct)
    {
        aniDirectL = (true) ^ (setup.setup.ani_direct);
        aniDirectR = (false) ^ (setup.setup.ani_direct);
    }
    else
    {
        aniDirectL = setup.setup.ani_direct;
        aniDirectR = setup.setup.ani_direct;
    }

    if(customAnimate) // User defined spriteSet (example: boss)
    {
        frameSequance = true;
        frames_listL = setup.setup.frames_left;
        frames_listR = setup.setup.frames_right;
        frameFirstL = 0;
        frameFirstR = 0;
        frameLastL = frames_listL.size() - 1;
        frameLastR = frames_listR.size() - 1;
    }
    else
    {
        //Standard animation
        switch(frameStyle)
        {
        case 2: //Left-Right-upper sprite
            framesCountOneSide = (int)setup.setup.frames * 4;
            //left
            frameFirstL = 0;
            frameLastL = (int)(framesCountOneSide - (framesCountOneSide / 4) * 3) - 1;
            //Right
            frameFirstR = (int)(framesCountOneSide - (framesCountOneSide / 4) * 3);
            frameLastR = (int)(framesCountOneSide / 2) - 1;
            break;

        case 1: //Left-Right sprite
            framesCountOneSide = (int)setup.setup.frames * 2;
            //left
            frameFirstL = 0;
            frameLastL = (int)(framesCountOneSide / 2) - 1;
            //Right
            frameFirstR = (int)(framesCountOneSide / 2);
            frameLastR = framesCountOneSide - 1;
            break;

        case 0: //Single sprite
        default:
            frameFirstL = 0;
            frameLastL = framesCountOneSide - 1;
            frameFirstR = 0;
            frameLastR = framesCountOneSide - 1;
            break;
        }
    }

#ifdef _DEBUG_
    WriteToLog(QtDebugMsg, QString("NPC-%1, framestyle is %2").arg(setup.id).arg(setup.framestyle));
#endif
    //curDirect  = dir;
    //setOffset(imgOffsetX+(-((double)localProps.gfx_offset_x)*curDirect), imgOffsetY );
    timer = new QTimer(this);
    timer->connect(
        timer, SIGNAL(timeout()),
        this,
        SLOT(nextFrame()));
    timer->setTimerType(Qt::PreciseTimer);
    //createAnimationFrames();
    setFrameL(getFrameNumL(frameFirstL));
    setFrameR(getFrameNumR(frameFirstR));
}

QPixmap AdvNpcAnimator::image(int dir, int frame)
{
    if((frame < 0) || (frame >= framesCountTotal))
    {
        if(dir <= 0)
            return mainImage->copy(0, CurrentFrameL * frame, frameWidth, frameHeight); //frames[CurrentFrameL];
        else
            return mainImage->copy(0, CurrentFrameR * frame, frameWidth, frameHeight); //frames[CurrentFrameR];
    }
    else
        return mainImage->copy(0, frameHeight * frame, frameWidth, frameHeight);
}

QPixmap &AdvNpcAnimator::wholeImage()
{
    return *mainImage;
}

QRect &AdvNpcAnimator::frameRect(int dir)
{
    if(dir <= 0)
        return frame_rect_L;
    else
        return frame_rect_R;
}

void AdvNpcAnimator::setFrameL(int y)
{
    //if(frames.isEmpty()) return;

    //Out of range protection
    if(y < frameFirstL) y = (frameLastL < 0) ? framesCountTotal - 1 : frameLastL;
    if(y >= framesCountTotal) y = (frameFirstL < framesCountTotal) ? frameFirstL : 0;
    CurrentFrameL = y;
    frame_rect_L.setRect(0, frameHeight * CurrentFrameL, frameWidth, frameHeight);
}

void AdvNpcAnimator::setFrameR(int y)
{
    //if(frames.isEmpty()) return;

    //Out of range protection
    if(y < frameFirstR) y = (frameLastR < 0) ? framesCountTotal - 1 : frameLastR;
    if(y >= framesCountTotal) y = (frameFirstR < framesCountTotal) ? frameFirstR : 0;
    CurrentFrameR = y;
    frame_rect_R.setRect(0, frameHeight * CurrentFrameR, frameWidth, frameHeight);
}

void AdvNpcAnimator::start()
{
    if(!animated) return;
    if((frameLastL > 0) && ((frameLastL - frameFirstL) <= 0)) return; //Don't start singleFrame animation
    if((frameLastR > 0) && ((frameLastR - frameFirstR) <= 0)) return; //Don't start singleFrame animation
    frameCurrentL = frameFirstL;
    frameCurrentR = frameFirstR;
    timer->start(frameSpeed);
}

void AdvNpcAnimator::stop()
{
    if(!animated) return;

    timer->stop();
    setFrameL(getFrameNumL(frameFirstL));
    setFrameR(getFrameNumR(frameFirstR));
}

void AdvNpcAnimator::nextFrame()
{
    //Left
    if(!aniDirectL)
    {
        //frameCurrent += frameSize * frameStep;
        frameCurrentL += frameStep;

        if(((frameCurrentL >= framesCountTotal - (frameStep - 1)) && (frameLastL <= -1)) ||
           ((frameCurrentL > frameLastL) && (frameLastL >= 0)))
        {
            if(!aniBiDirect)
                frameCurrentL = frameFirstL;
            else
            {
                frameCurrentL -= frameStep * 2;
                aniDirectL = !aniDirectL;
            }
        }
    }
    else
    {
        //frameCurrent -= frameSize * frameStep;
        frameCurrentL -= frameStep;

        if(frameCurrentL < frameFirstL)
        {
            if(!aniBiDirect)
                frameCurrentL = ((frameLastL == -1) ? framesCountTotal - 1 : frameLastL);
            else
            {
                frameCurrentL += frameStep * 2;
                aniDirectL = !aniDirectL;
            }
        }
    }

    setFrameL(getFrameNumL(frameCurrentL));

    //Right
    if(!aniDirectR)
    {
        //frameCurrent += frameSize * frameStep;
        frameCurrentR += frameStep;

        if(((frameCurrentR >= framesCountTotal - (frameStep - 1)) && (frameLastR <= -1)) ||
           ((frameCurrentR > frameLastR) && (frameLastR >= 0)))
        {
            if(!aniBiDirect)
                frameCurrentR = frameFirstR;
            else
            {
                frameCurrentR -= frameStep * 2;
                aniDirectR = !aniDirectR;
            }
        }
    }
    else
    {
        //frameCurrent -= frameSize * frameStep;
        frameCurrentR -= frameStep;

        if(frameCurrentR < frameFirstR)
        {
            if(!aniBiDirect)
                frameCurrentR = ((frameLastR == -1) ? framesCountTotal - 1 : frameLastR);
            else
            {
                frameCurrentR += frameStep * 2;
                aniDirectR = !aniDirectR;
            }
        }
    }

    setFrameR(getFrameNumR(frameCurrentR));
    emit onFrame();
}

int AdvNpcAnimator::getFrameNumL(int num)
{
    if(frameSequance)
    {
        if((num < 0) || (num >= frames_listL.size()))
            num = 0;
        return frames_listL[num];
    }
    else
        return num;
}

int AdvNpcAnimator::getFrameNumR(int num)
{
    if(frameSequance)
    {
        if((num < 0) || (num >= frames_listR.size()))
            num = 0;
        return frames_listR[num];
    }
    else
        return num;
}
