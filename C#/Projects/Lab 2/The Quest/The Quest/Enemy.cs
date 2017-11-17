using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public abstract class Enemy : Mover
    {
        private const int NearPlayerDistance = 25;
        private int hitPoints;
        public int HitPoints { get { return hitPoints; } }

        public bool Dead{
            get {
                if(hitPoints <= 0) return true;
                else return false;
            }
        }

        public Enemy(Game game, Point loc, int hitPts)
            : base(game, loc)
        {
            hitPoints = hitPts;
        }

        //all subclasses of enemy must implement this
        public abstract void Move(Random random);

        public void Hit(int maxDamage, Random random)
        {
            hitPoints -= random.Next(1, maxDamage);
        }

        protected bool NearPlayer()
        {
            return (Nearby(game.PlayerLocation, NearPlayerDistance));
        }

        protected Direction FindPlayerDirection(Point playerLoc)
        {
            Direction dirToMove;

            if (playerLoc.X > location.X + 10)
                dirToMove = Direction.Right;
            else if (playerLoc.X < location.X - 10)
                dirToMove = Direction.Left;
            else if (playerLoc.Y < location.Y - 10)
                dirToMove = Direction.Up;
            else
                dirToMove = Direction.Down;

            return dirToMove;
        }
    }
}
