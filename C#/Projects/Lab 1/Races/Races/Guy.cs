using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Races
{
    public class Guy
    {
        public string Name; //the guy's name
        public Bet MyBet;   //an instance of Bet() that has his bet
        public int Cash;    //how much cash he has

        //the last two fields are the guy's GUI controls on the form
        public RadioButton MyRadioButton;   
        public Label MyLabel;

        public void UpdateLabels()
        {
            MyLabel.Width = 190;
            MyLabel.Text = MyBet.GetDescription();
            MyRadioButton.Text = Name + " has " + Cash + " bucks";
        }

        public void ClearBet()
        {
            MyBet.Amount = 0;
        }

        public bool PlaceBet(int amount, int dog)
        {
            MyBet.Amount = amount;
            MyBet.Dog = dog;
            MyBet.Bettor = this;

            if (MyBet.Amount <= Cash)
            {
                Cash -= MyBet.Amount;
                return true;
            }
            else
            {
                MessageBox.Show("You don't have that much money to bet.", "Not Enough $$$");
                return false;
            }
        }

        public void Collect(int winner) {
            //after race is over, payout (whether you win or lose)
            Cash += MyBet.PayOut(winner);
            //...then update the labels
            UpdateLabels();
        }
    }
}
