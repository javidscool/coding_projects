//
//  ViewController.swift
//  iTahDoodle
//
//  Created by Javid Shamloo on 2/8/16.
//  Copyright © 2016 BigNerdRanch. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITableViewDelegate{
    
    //I DID THE BRONZE, SILVER, AND GOLD CHALLENGES
    
    @IBOutlet var itemTextField: UITextField!
    @IBOutlet var tableView: UITableView!
    
    let todoList = TodoList()

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        //use our ToDoList cell info to register the cells
        tableView.register(todoList.getCellInfo.cellClass,
                           forCellReuseIdentifier: todoList.getCellInfo.reuseIdentifier)
        tableView.dataSource = todoList
        tableView.delegate = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func addButtonPressed(_ sender: UIButton){
        guard let todo = itemTextField.text, !todo.isEmpty else{
            print("CAN'T ADD EMPTY TEXT!")
            return
        }
        
        todoList.add(todo)
        tableView.reloadData()
        itemTextField.text = ""
    }

    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        todoList.remove(indexPath)
        tableView.reloadData()
    }
}

