using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Bat : Enemy
    {
        public Bat(Game game, Point loc) : base(game, loc, 6) { }

        public override void Move(Random random)
        {
            if (HitPoints >= 1)
            {
                //50% chance it'll move twords the player
                if (random.Next(2) == 1)
                {
                    //find the direction to move...
                    Direction dirToMove = FindPlayerDirection(game.PlayerLocation);
                    //...then move in that direction
                    location = Move(dirToMove, game.Boundaries);
                }
                //50% chance it'll move in a random direction
                else
                {
                    switch (random.Next(1, 5))
                    {
                        case 1: Move(Direction.Down, game.Boundaries); break;
                        case 2: Move(Direction.Left, game.Boundaries); break;
                        case 3: Move(Direction.Right, game.Boundaries); break;
                        case 4: Move(Direction.Up, game.Boundaries); break;
                    }
                }

                //if i'm near the player, hit 'em
                if (NearPlayer())
                {
                    game.HitPlayer(2, random);
                }
            }
        }
    }
}
