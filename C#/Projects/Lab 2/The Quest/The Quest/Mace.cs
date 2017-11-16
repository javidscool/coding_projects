using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Mace : Weapon
    {
        public Mace(Game game, Point loc) : base(game, loc) { }

        public override string Name
        {
            get { return "Mace"; }
        }

        public override void Attack(Direction dir, Random random)
        {
            foreach (Enemy enemy in game.Enemies)
            {
                //assuming radius of 60
                if (Math.Sqrt(Math.Pow((enemy.Location.X - game.PlayerLocation.X), 2) + Math.Pow((enemy.Location.Y - game.PlayerLocation.Y), 2)) <= 60)
                    enemy.Hit(6, random);
            }
        }
    }
}
