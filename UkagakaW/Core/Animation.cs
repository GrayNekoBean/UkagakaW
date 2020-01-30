#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2019 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: Animation
* Author: Biobean
* Create On: 2019/12/5 20:49:44
*
* Overall Description:
* None
*
* Update Description:
* None
*
*
* ---------------------------------------------------
*/

#endregion

using UkagakaW.Render;

namespace UkagakaW.Core
{
    public class Animation
    {
        public string animationID;
        public AnimationState defaultState;

        public Animation(string animID)
        {
            this.animationID = animID;
        }
    }
}
