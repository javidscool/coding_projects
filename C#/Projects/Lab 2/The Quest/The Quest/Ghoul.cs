using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Ghoul : Enemy
    {
        public Ghoul(Game game, Point loc) : base(game, loc, 10) { }

        public override void Move(Random random)
        {
            if (HitPoints >= 1)
            {
                //2 in 3 chance it'll move twords the player
                if (random.Next(3) != 1)
                {
                    //find the direction to move...
                    Direction dirToMove = FindPlayerDirection(game.PlayerLocation);
                    //...then move in that direction
                    location = Move(dirToMove, game.Boundaries);
                }

                //if i'm near the player, hit 'em
                if (NearPlayer())
                {
                    game.HitPlayer(4, random);
                }
            }
        }
    }
}
