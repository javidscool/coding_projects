using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Space_Invaders
{
    public enum Direction
    {
        Left,
        Right,
        Up,
        Down
    }

    public class Game
    {
        private int score = 0;
        private int wave = 0;
        private int livesLeft = 0;
        private int framesSkipped = 0;
        private int frameNum = 0;

        private Rectangle boundaries;
        private Random random;

        private Direction invaderDirection;
        private List<Invader> invaders;

        private PlayerShip playerShip;
        private List<Shot> playerShots;
        private List<Shot> invaderShots;

        private Stars stars;

        public event EventHandler GameOver;
        public void OnGameOver(EventArgs e)
        {
            if (GameOver != null)
                GameOver(this, e);
        }


        public Game(Rectangle bound, Random rand)
        {
            boundaries = bound;
            random = rand;

            //create invaders
            invaders = new List<Invader>();
            InitializeImages();
            CreateWave();

            playerShip = new PlayerShip(boundaries, new Point(Math.Abs(boundaries.Left - boundaries.Right) / 2, boundaries.Bottom - 60));

            playerShots = new List<Shot>();
            invaderShots = new List<Shot>();

            stars = new Stars(boundaries, random);

            livesLeft = 2;
            framesSkipped = 6;
            frameNum = 0;
            wave = 1;
            invaderDirection = Direction.Left;
            
        }

        public bool ShipAlive{
            get { return playerShip.Alive; }
        }

        private static Bitmap ResizeImage(Image ImageToResize, int Width, int Height)
        {
            Bitmap bitmap = new Bitmap(Width, Height);
            using (Graphics graphics = Graphics.FromImage(bitmap))
            {
                graphics.DrawImage(ImageToResize, 0, 0, Width, Height);
            }
            return bitmap;
        }

        Bitmap[] satellites;
        Bitmap[] bugs;
        Bitmap[] spaceships;
        Bitmap[] flyingsaucers;
        Bitmap[] en_stars;

        private void InitializeImages()
        {
            satellites = new Bitmap[4];
            satellites[0] = ResizeImage(Properties.Resources.satellite1, 40, 40);
            satellites[1] = ResizeImage(Properties.Resources.satellite2, 40, 40);
            satellites[2] = ResizeImage(Properties.Resources.satellite3, 40, 40);
            satellites[3] = ResizeImage(Properties.Resources.satellite4, 40, 40);

            bugs = new Bitmap[4];
            bugs[0] = ResizeImage(Properties.Resources.bug1, 40, 40);
            bugs[1] = ResizeImage(Properties.Resources.bug2, 40, 40);
            bugs[2] = ResizeImage(Properties.Resources.bug3, 40, 40);
            bugs[3] = ResizeImage(Properties.Resources.bug4, 40, 40);

            spaceships = new Bitmap[4];
            spaceships[0] = ResizeImage(Properties.Resources.spaceship1, 40, 40);
            spaceships[1] = ResizeImage(Properties.Resources.spaceship2, 40, 40);
            spaceships[2] = ResizeImage(Properties.Resources.spaceship3, 40, 40);
            spaceships[3] = ResizeImage(Properties.Resources.spaceship4, 40, 40);

            flyingsaucers = new Bitmap[4];
            flyingsaucers[0] = ResizeImage(Properties.Resources.flyingsaucer1, 40, 40);
            flyingsaucers[1] = ResizeImage(Properties.Resources.flyingsaucer2, 40, 40);
            flyingsaucers[2] = ResizeImage(Properties.Resources.flyingsaucer3, 40, 40);
            flyingsaucers[3] = ResizeImage(Properties.Resources.flyingsaucer4, 40, 40);

            en_stars = new Bitmap[4];
            en_stars[0] = ResizeImage(Properties.Resources.star1, 40, 40);
            en_stars[1] = ResizeImage(Properties.Resources.star2, 40, 40);
            en_stars[2] = ResizeImage(Properties.Resources.star3, 40, 40);
            en_stars[3] = ResizeImage(Properties.Resources.star4, 40, 40);
        }

        public Bitmap ReturnImage(Invader inv, int animationCell)
        {
            if(inv.InvaderType == Type.Bug) return bugs[animationCell];
            else if(inv.InvaderType == Type.Satellite) return satellites[animationCell];
            else if(inv.InvaderType == Type.Saucer) return flyingsaucers[animationCell];
            else if(inv.InvaderType == Type.Spaceship) return spaceships[animationCell];
            //else it's a star
            else return en_stars[animationCell];
        }

        public void Draw(Graphics g, int animationCell)
        {
            frameNum++;

            g.FillRectangle(Brushes.Black, boundaries);

            stars.Draw(g);

            foreach (Invader zim in invaders)
                zim.Draw(g, animationCell);
            
            playerShip.Draw(g);
            
            foreach (Shot plToInv in playerShots)
                plToInv.Draw(g);
            
            foreach (Shot invToPl in invaderShots)
                invToPl.Draw(g);

            //drawing the score
            using (Font arial24Bold = new Font("Arial", 24, FontStyle.Bold))
            {
                g.DrawString(score.ToString(), arial24Bold, Brushes.SkyBlue, 0, 0);
            }

            //drawing the players ships
            int i = 50;
            int shipsToDraw = livesLeft;
            while (shipsToDraw > 0)
            {
                g.DrawImageUnscaled(ResizeImage(Properties.Resources.player, 30, 30), boundaries.Right - i, boundaries.Top);
                i += 50;
                shipsToDraw--;
            }
        }

        public void Twinkle()
        {
            stars.Twinkle(random);
        }

        public void MovePlayer(Direction dir)
        {
            if (playerShip.Alive)
                playerShip.Move(dir);
        }

        public void FireShot()
        {
            if (playerShots.Count < 2)
            {
                Point pt = new Point((playerShip.Area.Left + playerShip.Area.Right) / 2,
                                     playerShip.Area.Top - 10);
                Shot newShot = new Shot(pt, Direction.Up, boundaries, 20);
                playerShots.Add(newShot);
            }
        }

        //method to create the next wave
        private void CreateWave()
        {

            for (int i = 1; i <= 6; i++)
            {
                invaders.Add(new Invader(Type.Satellite, new Point(boundaries.Left + (i * 100), boundaries.Top + 50), this, 50));
            }

            for (int i = 1; i <= 6; i++)
            {
                invaders.Add(new Invader(Type.Bug, new Point(boundaries.Left + (i * 100), boundaries.Top + 150), this, 40));
            }


            for (int i = 1; i <= 6; i++)
            {
                invaders.Add(new Invader(Type.Spaceship, new Point(boundaries.Left + (i * 100), boundaries.Top + 250), this, 30));
            }

            for (int i = 1; i <= 6; i++)
            {
                invaders.Add(new Invader(Type.Saucer, new Point(boundaries.Left + (i * 100), boundaries.Top + 350), this, 20));
            }

            for (int i = 1; i <= 6; i++)
            {
                invaders.Add(new Invader(Type.Star, new Point(boundaries.Left + (i * 100), boundaries.Top + 450), this, 10));
            }


        }

        private void NextWave()
        {
            //create the next wave of invaders and get them moving twords the right hand side of the screen
            wave++;
            framesSkipped--;
            frameNum = 0;
            CreateWave();
            invaderDirection = Direction.Left;
        }

        public void Go()
        {
            //if the ship is alive, keep gameplay going
            if (playerShip.Alive)
            {
                if (invaders.Count == 0) NextWave();

                //move all the invaders
                MoveInvaders();

                //if any invader reaches the bottom of the screen, end the game
                foreach (Invader conquer in invaders)
                {
                    if (conquer.Area.Bottom >= playerShip.Area.Top)
                    {
                        EventArgs ev = new EventArgs();
                        //raise the GameOver event
                        OnGameOver(ev);
                    }
                }

                //allow invaders to return fire
                ReturnFire();

                //updating the player's shots
                for (int i = 0; i < playerShots.Count; i++ )
                {
                    if (!(playerShots[i].Move()))
                        playerShots.RemoveAt(i);
                    else
                    {
                        //check for collisions
                        CheckForInvaderCollisions(i);
                    }
                }


                //updating the invader's shots
                for (int i = 0; i < invaderShots.Count; i++)
                {
                    if ( !(invaderShots[i].Move()) )
                        invaderShots.RemoveAt(i);
                    else
                        CheckForPlayerCollisions(i);
                }

                
            }

        }

        private void MoveInvaders()
        {
            //return if frame should be skipped
            if (!(frameNum % framesSkipped == 0)) return;

            int row_switch = 0;
            switch (invaderDirection)
            {
                //if the invaders are moving to the right 
                case Direction.Right:
                    row_switch = 0;

                    //checking to see in invader hit the right-hand boundary
                    foreach(Invader zim in invaders)
                        if( (boundaries.Right - zim.Area.Right) <= 10){
                            row_switch = 1;
                            break;
                        }

                    //if the invader hit the right-hand boundary, move down and to the left
                    if (row_switch == 1)
                    {
                        foreach (Invader inv in invaders)
                            inv.Move(Direction.Down);
                        invaderDirection = Direction.Left;
                    }
                    //else, keep moving right
                    else foreach (Invader inv in invaders)
                            inv.Move(Direction.Right);
                    break;

                //if the invaders are moving to the left
                case Direction.Left:
                    row_switch = 0;

                    //checking to see in invader hit the left-hand boundary
                    foreach (Invader zim in invaders)
                        if ((zim.Area.Left - boundaries.Left) <= 10)
                        {
                            row_switch = 1;
                            break;
                        }

                    //if the invader hit the left-hand boundary, move down and to the right
                    if (row_switch == 1)
                    {
                        foreach (Invader inv in invaders)
                            inv.Move(Direction.Down);
                        invaderDirection = Direction.Right;
                    }

                    //else, keep moving left
                    else foreach (Invader inv in invaders)
                            inv.Move(Direction.Left);
                    break;
            }
              
        }

        private void ReturnFire()
        {
            //return if frame should be skipped
            if (!(frameNum % framesSkipped == 0)) return;

            //if the invaders can't shoot anymore, return
            if (invaderShots.Count >= (wave + 1)) return;
            
            //this makes it so that the invaders fire at random
            if (random.Next(10) < 10 - wave) return;

            //group the invaders by columns
            var invGroup =
                from inv in invaders
                group inv by inv.Location.X
                    into invGroups
                    orderby invGroups.Key descending
                    select invGroups;
            int choice = random.Next(invGroup.Count());

            //adding a new shot that's a little below the middle of the invader
            invaderShots.Add(new Shot(
                new Point( (invGroup.ElementAt(choice).Last().Area.Left + invGroup.ElementAt(choice).First().Area.Right) / 2,
                          invGroup.ElementAt(choice).Last().Area.Bottom + 10), Direction.Down, boundaries, 8));
        }


        private void CheckForInvaderCollisions(int shotNum)
        {
            Shot s = playerShots[shotNum];
            for(int i = 0; i < invaders.Count; i++)
                if (invaders[i].Area.Contains(s.Location))
                {
                    playerShots.RemoveAt(shotNum);
                    score += invaders[i].Score;
                    invaders.RemoveAt(i);
                    break;
                }

        }

        private void CheckForPlayerCollisions(int shotNum)
        {
            Shot s = invaderShots[shotNum];
           
            if (playerShip.Area.Contains(s.Location))
            {
                playerShip.Alive = false;
                livesLeft--;
                if (livesLeft < 0)
                {
                    EventArgs ev = new EventArgs();
                    //raise the GameOver event
                    OnGameOver(ev);
                }
                invaderShots.RemoveAt(shotNum);
            }
        }

    }
}
