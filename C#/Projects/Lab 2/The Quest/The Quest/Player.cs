using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace The_Quest
{
    public class Player : Mover
    {
        private Weapon equippedWeapon;
        public Weapon EquippedWeapon { get { return equippedWeapon; } }
        private int hitPoints;
        public int HitPoints { get { return hitPoints; } }

        private List<Weapon> inventory = new List<Weapon>();
        public List<string> Weapons
        {
            get
            {
                List<string> names = new List<string>();
                foreach (Weapon weapon in inventory)
                    names.Add(weapon.Name);
                return names;
            }
        }

        public Player(Game game, Point loc)
            : base(game, loc)
        {
            hitPoints = 10;
        }

        //when player's hit, decrease his hitPoints by a random amount
        public void Hit(int maxDamage, Random random)
        {
            hitPoints -= random.Next(1, maxDamage);
        }

        //when a player drinks a potion, increase his hitPoints by a random amount
        public void IncreaseHealth(int health, Random random)
        {
            hitPoints += random.Next(1, health);
        }

        public void Equip(string weaponName)
        {
            foreach (Weapon weapon in inventory)
            {
                if (weapon.Name == weaponName) equippedWeapon = weapon;
            }
        }

        public void Move(Direction dir)
        {
            location = Move(dir, game.Boundaries);
            if (!game.WeaponInRoom.PickedUp)
            {
                //see if the weapon is nearby and possibly pick it up
                if (Math.Sqrt(Math.Pow(location.X - game.WeaponInRoom.Location.X, 2) + Math.Pow(location.Y - game.WeaponInRoom.Location.Y, 2)) <= 10)
                {
                    inventory.Add(game.WeaponInRoom);
                    game.WeaponInRoom.PickUpWeapon();
                    if (inventory.Count == 1)
                        Equip(inventory[0].Name);
                }
            }
        }

        public void Attack(Direction dir, Random random)
        {
            //figure out which weapon is equipped and use it's attack method
            //don't do shit if no weapon is equipped
            //if you use a potion remove it from the inventory
            if (equippedWeapon != null)
            {
                equippedWeapon.Attack(dir, random);
                if (equippedWeapon is IPotion)
                    inventory.Remove(equippedWeapon);
            }

        }


    }
}
