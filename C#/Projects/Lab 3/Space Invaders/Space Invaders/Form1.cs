using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Space_Invaders
{
    public partial class Form1 : Form
    {
        //GameOver bool that checks when the game's over
        bool gameOver;
        Game game;

        Random random;

        public Form1()
        {
            InitializeComponent();
            random = new Random();
            game = new Game(ClientRectangle, random);
            game.GameOver += new EventHandler(game_GameOver);
            animationTimer.Start();
            gameTimer.Start();
        }

        private int cell = 0;
        private int frame = 0;
        private int skip = 0;
        //the animation tick event
        private void animationTimer_Tick(object sender, EventArgs e)
        {
            skip++;
            if (skip == 3)
            {
                frame++;
                if (frame >= 6) frame = 0;
                switch (frame)
                {
                    case 0: cell = 0; break;
                    case 1: cell = 1; break;
                    case 2: cell = 2; break;
                    case 3: cell = 3; break;
                    case 4: cell = 2; break;
                    case 5: cell = 1; break;
                }
                skip = 0;
            }

            game.Twinkle();
            Refresh();
        }


        List<Keys> keysPressed = new List<Keys>();
        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            //if the user presses Q, exit
            if (e.KeyCode == Keys.Q)
            {
                if (!gameOver)
                {
                    gameTimer.Stop();
                    DialogResult result = MessageBox.Show("Are you sure you want to quit?", "Invaders", MessageBoxButtons.YesNo);
                    if (result == DialogResult.Yes)
                        Application.Exit();
                    else gameTimer.Start();
                }
                else Application.Exit();
            }

            if(gameOver)
                if (e.KeyCode == Keys.S)
                {
                    gameOver = false;
                    game = new Game(ClientRectangle, random);
                    game.GameOver += new EventHandler(game_GameOver);
                    gameTimer.Start();
                    return;
                }

            //spacebar fires a shot
            if (e.KeyCode == Keys.Space)
                game.FireShot();

            //by removing the key and then readding it, we make the key the last (most current) item in the list
            //this is in case the player mashes a few keys at the same time
            if (keysPressed.Contains(e.KeyCode))
                keysPressed.Remove(e.KeyCode);
            keysPressed.Add(e.KeyCode);
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
              if (keysPressed.Contains(e.KeyCode))
                keysPressed.Remove(e.KeyCode);
        }

        //the game timer tick event
        private void gameTimer_Tick(object sender, EventArgs e)
        {
            if (keysPressed.Count() >= 1)
            {
                switch (keysPressed[0])
                {
                    case Keys.Left:
                        game.MovePlayer(Direction.Left);
                        break;
                    case Keys.Right:
                        game.MovePlayer(Direction.Right);
                        break;
                    case Keys.Q:
                        keysPressed.Clear();
                        break;
                }
            }
            if(game.ShipAlive)
                game.Go();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            game.Draw(e.Graphics, cell);

            if (gameOver)
            {
                //write game over in middle of screen if the game's over
                using (Font arial28Bold = new Font("Arial", 28, FontStyle.Bold))
                using (Font arial16Bold = new Font("Arial", 16, FontStyle.Bold))
                {
                    //drawing Game Over on the screen
                    g.DrawString("GAME OVER", arial28Bold, Brushes.WhiteSmoke, (ClientRectangle.Right - ClientRectangle.Left) / 2 - 100, (ClientRectangle.Bottom - ClientRectangle.Top) / 2);

                    //write: "S to start new game, Q to quit" in lower right-hand corner
                    string startOver = "Press S to start a new game or Q to Quit";
                    //Size stringSize = Size.Ceiling(g.MeasureString(startOver, arial16Bold));
                    g.DrawString(startOver, arial16Bold, Brushes.WhiteSmoke, ClientRectangle.Right - 415, ClientRectangle.Bottom - 25);
                }
                
            }
            
        }

        void game_GameOver(object sender, EventArgs e)
        {
            gameTimer.Stop();
            gameOver = true;
            Invalidate();
        }

    }
}
