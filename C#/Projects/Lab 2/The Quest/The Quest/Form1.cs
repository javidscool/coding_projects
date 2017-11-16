using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace The_Quest
{
    public partial class Form1 : Form
    {
        private Game game;
        private Random random = new Random();
        Renderer renderer;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            game = new Game(new Rectangle(78, 57, 420, 155));
            game.NewLevel(random);
            UpdateCharacters();
            renderer = new Renderer(game);
        }

        private void sword_Click(object sender, EventArgs e)
        {
            if (game.CheckPlayerInventory("Sword"))
            {
                if (attackUp.Text == "Drink")
                {
                    attackDown.Enabled = true;
                    attackLeft.Enabled = true;
                    attackRight.Enabled = true;
                    attackUp.Text = "Up";
                }
                game.Equip("Sword");
                sword.BorderStyle = BorderStyle.FixedSingle;
                redPotion.BorderStyle = BorderStyle.None;
                bow.BorderStyle = BorderStyle.None;
                bluePotion.BorderStyle = BorderStyle.None;
                mace.BorderStyle = BorderStyle.None;
            }

        }

        private void redPotion_Click(object sender, EventArgs e)
        {
            if (game.CheckPlayerInventory("Red Potion"))
            {
                game.Equip("Red Potion");
                sword.BorderStyle = BorderStyle.None;
                redPotion.BorderStyle = BorderStyle.FixedSingle;
                bow.BorderStyle = BorderStyle.None;
                bluePotion.BorderStyle = BorderStyle.None;
                mace.BorderStyle = BorderStyle.None;
                attackDown.Enabled = false;
                attackLeft.Enabled = false;
                attackRight.Enabled = false;
                attackUp.Text = "Drink";
            }
        }

        private void bow_Click(object sender, EventArgs e)
        {
            if (game.CheckPlayerInventory("Bow"))
            {
                if (attackUp.Text == "Drink")
                {
                    attackDown.Enabled = true;
                    attackLeft.Enabled = true;
                    attackRight.Enabled = true;
                    attackUp.Text = "Up";
                }
                game.Equip("Bow");
                sword.BorderStyle = BorderStyle.None;
                redPotion.BorderStyle = BorderStyle.None;
                bow.BorderStyle = BorderStyle.FixedSingle;
                bluePotion.BorderStyle = BorderStyle.None;
                mace.BorderStyle = BorderStyle.None;
            }
        }

        private void bluePotion_Click(object sender, EventArgs e)
        {
            if (game.CheckPlayerInventory("Blue Potion"))
            {
                game.Equip("Blue Potion");
                sword.BorderStyle = BorderStyle.None;
                redPotion.BorderStyle = BorderStyle.None;
                bow.BorderStyle = BorderStyle.None;
                bluePotion.BorderStyle = BorderStyle.FixedSingle;
                mace.BorderStyle = BorderStyle.None;
                attackDown.Enabled = false;
                attackLeft.Enabled = false;
                attackRight.Enabled = false;
                attackUp.Text = "Drink";
            }
        }

        private void mace_Click(object sender, EventArgs e)
        {
            if (game.CheckPlayerInventory("Mace"))
            {
                if (attackUp.Text == "Drink")
                {
                    attackDown.Enabled = true;
                    attackLeft.Enabled = true;
                    attackRight.Enabled = true;
                    attackUp.Text = "Up";
                }
                game.Equip("Mace");
                sword.BorderStyle = BorderStyle.None;
                redPotion.BorderStyle = BorderStyle.None;
                bow.BorderStyle = BorderStyle.None;
                bluePotion.BorderStyle = BorderStyle.None;
                mace.BorderStyle = BorderStyle.FixedSingle;
            }
        }

        private void moveLeft_Click(object sender, EventArgs e)
        {
            game.Move(Direction.Left, random);
            UpdateCharacters();
        }

        private void moveUp_Click(object sender, EventArgs e)
        {
            game.Move(Direction.Up, random);
            UpdateCharacters();
        }

        private void moveRight_Click(object sender, EventArgs e)
        {
            game.Move(Direction.Right, random);
            UpdateCharacters();
        }

        private void moveDown_Click(object sender, EventArgs e)
        {
            game.Move(Direction.Down, random);
            UpdateCharacters();
        }

        private void attackUp_Click(object sender, EventArgs e)
        {
            game.Attack(Direction.Up, random);
            UpdateCharacters();
        }

        private void attackLeft_Click(object sender, EventArgs e)
        {
            game.Attack(Direction.Left, random);
            UpdateCharacters();
        }

        private void attackRight_Click(object sender, EventArgs e)
        {
            game.Attack(Direction.Right, random);
            UpdateCharacters();
        }

        private void attackDown_Click(object sender, EventArgs e)
        {
            game.Attack(Direction.Down, random);
            UpdateCharacters();
        }

        public void UpdateCharacters()
        {
            playerHitPoints.Text = game.PlayerHitPoints.ToString();

            bool showBat = false;
            bool showGhost = false;
            bool showGhoul = false;
            int enemiesShown = 0;

            foreach (Enemy enemy in game.Enemies)
            {
                if (enemy is Bat)
                {
                    batHitPoints.Text = enemy.HitPoints.ToString();
                    if (enemy.HitPoints > 0)
                    {
                        showBat = true;
                        enemiesShown++;
                    }
                }

                if (enemy is Ghost)
                {
                    ghostHitPoints.Text = enemy.HitPoints.ToString();
                    if (enemy.HitPoints > 0)
                    {
                        showGhost = true;
                        enemiesShown++;
                    }
                }

                if (enemy is Ghoul)
                {
                    ghoulHitPoints.Text = enemy.HitPoints.ToString();
                    if (enemy.HitPoints > 0)
                    {
                        showGhoul = true;
                        enemiesShown++;
                    }
                }
            }

            //for automatic pick-up of first weapon
            //we already equipped it in player's Move() method, this is just to draw the box around the weapon
            if(game.PlayerWeapons.Count == 1)
                if (game.PlayerEquippedWeapon.Name == "Sword")
                {
                    sword.BorderStyle = BorderStyle.FixedSingle;
                    redPotion.BorderStyle = BorderStyle.None;
                    bow.BorderStyle = BorderStyle.None;
                    bluePotion.BorderStyle = BorderStyle.None;
                    mace.BorderStyle = BorderStyle.None;
                }


            if (showBat == false)
                batHitPoints.Text = "";
            if (showGhost == false)
                ghostHitPoints.Text = "";
            if (showGhoul == false)
                ghoulHitPoints.Text = "";

            if (game.CheckPlayerInventory("Sword"))
                sword.Visible = true;
            else
                sword.Visible = false;

            if (game.CheckPlayerInventory("Bow"))
                bow.Visible = true;
            else
                bow.Visible = false;

            if (game.CheckPlayerInventory("Mace"))
                mace.Visible = true;
            else
                mace.Visible = false;

            if (game.CheckPlayerInventory("Blue Potion"))
                bluePotion.Visible = true;
            else
                bluePotion.Visible = false;

            if (game.CheckPlayerInventory("Red Potion"))
                redPotion.Visible = true;
            else
                redPotion.Visible = false;

            if (game.PlayerHitPoints <= 0)
            {
                MessageBox.Show("You died.");
                MessageBox.Show("Loser!!");
                Application.Exit();
            }

            if (enemiesShown < 1)
            {
                MessageBox.Show("You have defeated every enemy in this level");
                bool game_continue = game.NewLevel(random);
                if(game_continue) UpdateCharacters();
            }

            //drawing all the new shit
            this.Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            renderer.PaintDungeon(e.Graphics);
        }
    }
}
