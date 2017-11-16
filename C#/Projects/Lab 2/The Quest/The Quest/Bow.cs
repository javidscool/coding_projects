using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Bow : Weapon
    {
        public Bow(Game game, Point loc) : base(game, loc) { }

        public override string Name
        {
            get { return "Bow"; }
        }

        public override void Attack(Direction dir, Random random)
        {
            if (dir == Direction.Up)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 125
                    if ((Math.Abs(enemy.Location.X - game.PlayerLocation.X) <= 15) && (enemy.Location.Y < game.PlayerLocation.Y) && Math.Abs(enemy.Location.Y - game.PlayerLocation.Y) <= 125)
                        enemy.Hit(1, random);
                }

            else if (dir == Direction.Down)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 125
                    if ((Math.Abs(enemy.Location.X - game.PlayerLocation.X) <= 15) && (enemy.Location.Y > game.PlayerLocation.Y) && Math.Abs(enemy.Location.Y - game.PlayerLocation.Y) <= 125)
                        enemy.Hit(1, random);
                }

            else if (dir == Direction.Left)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 125
                    if ((Math.Abs(enemy.Location.Y - game.PlayerLocation.Y) <= 15) && (enemy.Location.X < game.PlayerLocation.X) && Math.Abs(enemy.Location.X - game.PlayerLocation.X) <= 125)
                        enemy.Hit(1, random);
                }

            else if (dir == Direction.Right)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 125
                    if ((Math.Abs(enemy.Location.Y - game.PlayerLocation.Y) <= 15) && (enemy.Location.X > game.PlayerLocation.X) && Math.Abs(enemy.Location.X - game.PlayerLocation.X) <= 125)
                        enemy.Hit(1, random);
                }
        }
    }
}
