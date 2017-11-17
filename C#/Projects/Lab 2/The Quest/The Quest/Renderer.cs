using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    public class Renderer
    {
        private Game game;
        private Bitmap Player;
        private Dictionary<string, Bitmap> EnemyPics;
        private Dictionary<string, Bitmap> WeaponPics;

        public Renderer(Game gme)
        {
            game = gme;
            EnemyPics = new Dictionary<string, Bitmap>();
            WeaponPics = new Dictionary<string, Bitmap>();
            InitializeImages();
        }

        private void InitializeImages()
        {
            Player = ResizeImage(Properties.Resources.player, 30, 30);
            EnemyPics.Add("bat", ResizeImage(Properties.Resources.bat, 30, 30));
            EnemyPics.Add("ghost", ResizeImage(Properties.Resources.ghost, 30, 30));
            EnemyPics.Add("ghoul", ResizeImage(Properties.Resources.ghoul, 30, 30));
            WeaponPics.Add("sword", ResizeImage(Properties.Resources.sword, 30, 30));
            WeaponPics.Add("bow", ResizeImage(Properties.Resources.bow, 30, 30));
            WeaponPics.Add("mace", ResizeImage(Properties.Resources.mace, 30, 30));
            WeaponPics.Add("blue", ResizeImage(Properties.Resources.potion_blue, 30, 30));
            WeaponPics.Add("red", ResizeImage(Properties.Resources.potion_red, 30, 30));
        }

        public static Bitmap ResizeImage(Bitmap picture, int width, int height)
        {
            Bitmap resizedPicture = new Bitmap(width, height);
            using (Graphics g = Graphics.FromImage(resizedPicture))
            {
                g.DrawImage(picture, 0, 0, width, height);
            }

            return resizedPicture;
        }

        public void PaintDungeon(Graphics g)
        {
            //drawing enemies
            foreach (Enemy enemy in game.Enemies)
            {
                if(!enemy.Dead)
                    if (enemy is Bat)
                        g.DrawImageUnscaled(EnemyPics["bat"], enemy.Location.X, enemy.Location.Y);
                    else if (enemy is Ghost)
                        g.DrawImageUnscaled(EnemyPics["ghost"], enemy.Location.X, enemy.Location.Y);
                    else if (enemy is Ghoul)
                        g.DrawImageUnscaled(EnemyPics["ghoul"], enemy.Location.X, enemy.Location.Y);
            }

            //drawing weapon in room
            if (!game.WeaponInRoom.PickedUp)
            {
                if (game.WeaponInRoom.Name == "Sword")
                    g.DrawImageUnscaled(WeaponPics["sword"], game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y);
                else if (game.WeaponInRoom.Name == "Bow")
                    g.DrawImageUnscaled(WeaponPics["bow"], game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y);
                else if (game.WeaponInRoom.Name == "Mace")
                    g.DrawImageUnscaled(WeaponPics["mace"], game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y);
                else if (game.WeaponInRoom.Name == "Blue Potion")
                    g.DrawImageUnscaled(WeaponPics["blue"], game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y);
                else if (game.WeaponInRoom.Name == "Red Potion")
                    g.DrawImageUnscaled(WeaponPics["red"], game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y);
            }
            else if(game.WeaponInRoom.PickedUp)
                g.FillRectangle(Brushes.Transparent, game.WeaponInRoom.Location.X, game.WeaponInRoom.Location.Y, 30, 30);
            
            //drawing player
            g.DrawImageUnscaled(Player, game.PlayerLocation.X, game.PlayerLocation.Y);
        }
    }
}
