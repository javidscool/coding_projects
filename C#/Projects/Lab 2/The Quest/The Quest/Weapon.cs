using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public abstract class Weapon : Mover
    {
        private bool pickedUp;
        public bool PickedUp { get { return pickedUp; } }

        public Weapon(Game game, Point loc) : base(game, loc)
        {
            pickedUp = false;
        }

        public void PickUpWeapon() { pickedUp = true; }

        public abstract string Name { get; }

        public abstract void Attack(Direction dir, Random random);

    }
}
