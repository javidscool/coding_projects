using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Races
{
    public class Bet
    {
        public int Amount;  //the amount of cash that was bet
        public int Dog;     //the number of the dog the bet is on
        public Guy Bettor;  //the buy who placed the bet

        public string GetDescription()
        {
            if (Amount > 0)
            {
                return Bettor.Name + " bets " + Amount + " on dog #" + Dog;
            }
            else return Bettor.Name + " hasn't placed a bet."; 
        }

        public int PayOut(int Winner)
        {
            if (Dog == Winner) return Amount;
            else return -Amount;
        }

    }
}
