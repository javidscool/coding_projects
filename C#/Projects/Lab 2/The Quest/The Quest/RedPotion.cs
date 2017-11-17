using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class RedPotion : Weapon, IPotion
    {
        private bool used;
        public bool Used { get { return used; } }

        public RedPotion(Game game, Point loc) : base(game, loc) { used = false; }

        public override string Name
        {
            get { return "Red Potion"; }
        }

        public override void Attack(Direction dir, Random random)
        {
            game.IncreasePlayerHealth(10, random);
            used = true;
        }
    }
}
