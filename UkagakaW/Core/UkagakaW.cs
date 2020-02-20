#region Statement

/*
* ---------------------Statement---------------------
* Copyright(c) 2018-2020 Bio-Electric Studio
* All Right Reserved
* This C# source file is for the Application of Project Ukagaka_W.
* You are not allowed to copy any code from here without permission.
*
* File: UkagakaW
* Author: Biobean
* Create On: 2020/2/9 16:25:54
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UkagakaW.Core
{
    public class UkagakaW
    {
        public static Ukagaka GetUkagaka(string UkagakaID)
        {
            return UkagakaWMain.ukagakaInstances[UkagakaID].ukagaka;
        }
    }
}
