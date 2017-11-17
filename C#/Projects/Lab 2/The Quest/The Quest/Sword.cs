using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Sword : Weapon
    {
        public Sword(Game game, Point loc) : base(game, loc) { }

        public override string Name
        {
            get { return "Sword"; }
        }

        public override void Attack(Direction dir, Random random)
        {
            if (dir == Direction.Up)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 35
                    if ((enemy.Location.Y < game.PlayerLocation.Y) && Math.Sqrt(Math.Pow((enemy.Location.X - game.PlayerLocation.X), 2) + Math.Pow((enemy.Location.Y - game.PlayerLocation.Y), 2)) <= 35)
                        enemy.Hit(3, random);
                }

            else if (dir == Direction.Down)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 35
                    if ((enemy.Location.Y > game.PlayerLocation.Y) && Math.Sqrt(Math.Pow((enemy.Location.X - game.PlayerLocation.X), 2) + Math.Pow((enemy.Location.Y - game.PlayerLocation.Y), 2)) <= 35)
                        enemy.Hit(3, random);
                }

            else if (dir == Direction.Right)
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 35
                    if ((enemy.Location.X > game.PlayerLocation.X) && Math.Sqrt(Math.Pow((enemy.Location.X - game.PlayerLocation.X), 2) + Math.Pow((enemy.Location.Y - game.PlayerLocation.Y), 2)) <= 35)
                        enemy.Hit(3, random);
                }

            //else enemy is to the left
            else
                foreach (Enemy enemy in game.Enemies)
                {
                    //assuming radius of 35
                    if ((enemy.Location.X < game.PlayerLocation.X) && Math.Sqrt(Math.Pow((enemy.Location.X - game.PlayerLocation.X), 2) + Math.Pow((enemy.Location.Y - game.PlayerLocation.Y), 2)) <= 35)
                        enemy.Hit(3, random);
                }
        }
    }
}
