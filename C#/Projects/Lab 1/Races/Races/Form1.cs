using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Races
{
    public partial class Form1 : Form
    {
        Guy[] guyarr = new Guy[3];
        Greyhound[] greyarr = new Greyhound[4];
        bool joe_bet = false;
        bool bob_bet = false;
        bool al_bet = false;

        public Form1()
        {
            InitializeComponent();
            
            guyarr[0] = new Guy() { Name = "Joe", MyBet = null, Cash = 100, MyLabel = JoeLabel, MyRadioButton = JoeRadio};
            guyarr[1] = new Guy() { Name = "Bob", MyBet = null, Cash = 100, MyLabel = BobLabel, MyRadioButton = BobRadio };
            guyarr[2] = new Guy() { Name = "Al", MyBet = null, Cash = 100, MyLabel = AlLabel, MyRadioButton = AlRadio };
   
            greyarr[0] = new Greyhound() { StartingPosition = 0, RaceTrackLength = 625, MyPictureBox = Dog1, Location = 0 };
            greyarr[1] = new Greyhound() { StartingPosition = 0, RaceTrackLength = 625, MyPictureBox = Dog2, Location = 0 };
            greyarr[2] = new Greyhound() { StartingPosition = 0, RaceTrackLength = 625, MyPictureBox = Dog3, Location = 0 };
            greyarr[3] = new Greyhound() { StartingPosition = 0, RaceTrackLength = 625, MyPictureBox = Dog4, Location = 0 };

            for (int i = 0; i < guyarr.Length; i++)
            {
                guyarr[i].MyBet = new Bet() { Amount = 0, Dog = 0, Bettor = guyarr[i] };
                guyarr[i].UpdateLabels();
            }
            for (int i = 0; i < greyarr.Length; i++)
                greyarr[i].TakeStartingPosition();

            MinBetLabel.Text = "Minimum bet: " + (int)DollarSlider.Minimum + " bucks";

        }

        private void BettorButton_Click(object sender, EventArgs e)
        {
            if (JoeRadio.Checked == true)
            {
                if (joe_bet == false)
                {
                    //I place a bet which takes away cash from the guy...
                    guyarr[0].PlaceBet((int)DollarSlider.Value, (int)DogSlider.Value);
                    //...then I update the labels so that I can show that the guy got his money taken when he placed the bet... 
                    guyarr[0].UpdateLabels();
                    //...then I give him his money back because if he loses, i'll take it away again and if he wins i'll add to it
                    //and if he loses it i'll take it away
                    //all this is just for visual purposes 
                    guyarr[0].Cash += guyarr[0].MyBet.Amount;
                    joe_bet = true;
                }
            }
            if (BobRadio.Checked == true)
            {
                if (bob_bet == false)
                {
                    //I place a bet which takes away cash from the guy...
                    guyarr[1].PlaceBet((int)DollarSlider.Value, (int)DogSlider.Value);
                    //...then I update the labels so that I can show that the guy got his money taken when he placed the bet... 
                    guyarr[1].UpdateLabels();
                    //...then I give him his money back because if he loses, i'll take it away again and if he wins i'll add to it
                    //and if he loses it i'll take it away
                    //all this is just for visual purposes 
                    guyarr[1].Cash += guyarr[1].MyBet.Amount;
                    bob_bet = true;
                }
            }
            if (AlRadio.Checked == true)
            {
                if (al_bet == false)
                {
                    //I place a bet which takes away cash from the guy...
                    guyarr[2].PlaceBet((int)DollarSlider.Value, (int)DogSlider.Value);
                    //...then I update the labels so that I can show that the guy got his money taken when he placed the bet... 
                    guyarr[2].UpdateLabels();
                    //...then I give him his money back because if he loses, i'll take it away again and if he wins i'll add to it
                    //and if he loses it i'll take it away
                    //all this is just for visual purposes 
                    guyarr[2].Cash += guyarr[2].MyBet.Amount;
                    al_bet = true;
                }
            }

        }

        private void RaceButton_Click(object sender, EventArgs e)
        {
            //when race is on, no more bets can be made and no other races can take place
            BettorButton.Enabled = false;
            RaceButton.Enabled = false;

            bool winner = false;    //bool that sees if we have a winner in the first place
            int win_dog;    //keeps track of the winner
            int i;

            while (winner != true)
            {
                System.Threading.Thread.Sleep(10);
                for (i = 0; i < greyarr.Length && winner != true; i++)
                {
                    winner = greyarr[i].Run();

                    //if we found a winner, pay out 
                    if (winner == true)
                    {
                        win_dog = (i + 1);   //add 1 because we start at index 0
                        MessageBox.Show("We have a winner - dog #" + win_dog);
                        for (int j = 0; j < guyarr.Length; j++)
                        {
                            //changing cash flow depending on whether player won or lost
                            guyarr[j].Collect(win_dog);

                            //then clear his bet and update the labels
                            guyarr[j].ClearBet();
                            guyarr[j].UpdateLabels();
                        }
                    }
                }
            }

            //after race is finished, start over
            for (int j = 0; j < greyarr.Length; j++)
                greyarr[j].TakeStartingPosition();

            //reset bet status'
            joe_bet = false;
            bob_bet = false;
            al_bet = false;

            //after race is over, you can place bets again and start a new race
            BettorButton.Enabled = true;
            RaceButton.Enabled = true;

        }

        private void JoeRadio_CheckedChanged(object sender, EventArgs e)
        {
            BettorLabel.Text = "Joe";
        }

        private void BobRadio_CheckedChanged(object sender, EventArgs e)
        {
            BettorLabel.Text = "Bob";
        }

        private void AlRadio_CheckedChanged(object sender, EventArgs e)
        {
            BettorLabel.Text = "Al";
        }

    }
}
