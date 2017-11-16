using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace Races
{
    public class Greyhound
    {
        public int StartingPosition;    //where MyPictureBox starts
        public int RaceTrackLength;     //how long the racetrack is
        public PictureBox MyPictureBox;  //my PictureBox object
        public int Location;    //my Location on the racetrack
        public static Random Randomizer = new Random();   //an single instance of Random

        public bool Run()
        {
            int distance;
            distance = Randomizer.Next(5);
            while (distance == 0) { distance = Randomizer.Next(5); }  
            Point p = MyPictureBox.Location;
            p.X += distance;
            Application.DoEvents();
            MyPictureBox.Location = p;
            if (MyPictureBox.Location.X >= RaceTrackLength)
            {
                return true;
            }
            else return false;
        }

        public void TakeStartingPosition()
        {
            StartingPosition = 30;
            Point p = MyPictureBox.Location;
            p.X = StartingPosition;
            MyPictureBox.Location = p;
        }

    }
}
