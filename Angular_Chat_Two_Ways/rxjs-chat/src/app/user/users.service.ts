import { Injectable } from '@angular/core';
import { Subject, BehaviorSubject } from 'rxjs';
import { User } from './user.model';


//UserService manages our current user
@Injectable()
export class UsersService {
	// `currentUser` contains the current user
	currentUser: Subject<User> = new BehaviorSubject<User>(null);

	public setCurrentUser(newUser: User): void {
		//next() is used to push a new value to the stream
		this.currentUser.next(newUser);
	}
}

export const userServiceInjectables: Array<any> = [ UsersService ];
