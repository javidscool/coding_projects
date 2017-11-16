using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace The_Quest
{
    public class Game
    {

        //public fields for the list of enemies and the one weapon in the room
        public List<Enemy> Enemies;
        public Weapon WeaponInRoom;

        private Player player;
        public Point PlayerLocation { get { return player.Location; } }
        public Weapon PlayerEquippedWeapon { get { return player.EquippedWeapon; } }
        public int PlayerHitPoints { get { return player.HitPoints; } }
        public List<string> PlayerWeapons { get { return player.Weapons; } }

        private int level = 0;
        public int Level { get { return level; } }

        private Rectangle boundaries;
        public Rectangle Boundaries { get { return boundaries; } }

        public Game(Rectangle bndries)
        {
            boundaries = bndries;
            player = new Player(this, new Point(boundaries.Left + 10, boundaries.Top + 70));
        }

        public void Move(Direction direction, Random random)
        {
            player.Move(direction);
            foreach (Enemy enemy in Enemies)
                enemy.Move(random);
        }

        public void Equip(string weaponName)
        {
            player.Equip(weaponName);
        }

        public bool CheckPlayerInventory(string weaponName)
        {
            return player.Weapons.Contains(weaponName);
        }

        public void HitPlayer(int maxDamage, Random random)
        {
            player.Hit(maxDamage, random);
        }

        public void IncreasePlayerHealth(int health, Random random)
        {
            player.IncreaseHealth(health, random);
        }

        public void Attack(Direction direction, Random random)
        {
            player.Attack(direction, random);
            foreach (Enemy enemy in Enemies)
                enemy.Move(random);
        }

        private Point GetRandomLocation(Random random)
        {
            return new Point(boundaries.Left + random.Next(boundaries.Right / 10 - boundaries.Left / 10) * 10,
                             boundaries.Top + random.Next(boundaries.Bottom / 10 - boundaries.Top / 10) * 10);
        }

        public bool NewLevel(Random random)
        {
            level++;
            switch (level)
            {
                case 1:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Bat(this,GetRandomLocation(random)));
                    WeaponInRoom = new Sword(this, GetRandomLocation(random));

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 2:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Ghost(this, GetRandomLocation(random)));
                    WeaponInRoom = new BluePotion(this, GetRandomLocation(random));

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 3:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Ghoul(this, GetRandomLocation(random)));
                    WeaponInRoom = new Bow(this, GetRandomLocation(random));

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 4:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Bat(this, GetRandomLocation(random)));
                    Enemies.Add(new Ghost(this, GetRandomLocation(random)));

                    if (!player.Weapons.Contains("Bow"))
                    {
                        WeaponInRoom = new Bow(this, GetRandomLocation(random));
                    }
                    else if (player.Weapons.Contains("Bow") && !player.Weapons.Contains("Blue Potion"))
                    {
                        WeaponInRoom = new BluePotion(this, GetRandomLocation(random));
                    }

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 5:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Bat(this, GetRandomLocation(random)));
                    Enemies.Add(new Ghoul(this, GetRandomLocation(random)));

                    WeaponInRoom = new RedPotion(this, GetRandomLocation(random));

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 6:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Ghost(this, GetRandomLocation(random)));
                    Enemies.Add(new Ghoul(this, GetRandomLocation(random)));
                    WeaponInRoom = new Mace(this, GetRandomLocation(random));

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 7:
                    //so we start with an empty list
                    Enemies = new List<Enemy>();

                    Enemies.Add(new Bat(this, GetRandomLocation(random)));
                    Enemies.Add(new Ghost(this, GetRandomLocation(random)));
                    Enemies.Add(new Ghoul(this, GetRandomLocation(random)));

                    if (!player.Weapons.Contains("Mace"))
                    {
                        WeaponInRoom = new Mace(this, GetRandomLocation(random));
                    }
                    else if (player.Weapons.Contains("Mace") && !player.Weapons.Contains("Red Potion"))
                    {
                        WeaponInRoom = new RedPotion(this, GetRandomLocation(random));
                    }

                    //redrawing the form
                    Form1.ActiveForm.Invalidate();
                    return true;

                case 8:
                    MessageBox.Show("Holy Shit!! You beat the game!");
                    Application.Exit();
                    return false;

                default: return false;
            }
        }

    }
}
