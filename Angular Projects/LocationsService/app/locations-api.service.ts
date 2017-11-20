import {Injectable} from '@angular/core';
import {Http, Headers, Request, RequestOptions} from '@angular/http';
import 'rxjs/add/operator/toPromise';

import {Location} from './location';

const URL = 'http://unraveling-ng.azurewebsites.net/api/dive/location/';
const USER_ID = 'f3c0c6bc-2d91-4ae3-8695-4bec0a9414e5';
const USER_SECRET = '708e7421a284447488c7901dd99cca7e625d22052a2043f0b61e3fbee0c2d389ba31777dc0124cadb85091081701678b';

@Injectable()
export class LocationsApiService {
	private locations: Location[] = [];

	constructor(private http: Http) { }

	getLocations() {
		let options = new RequestOptions({
			headers: this.getHeaders()
		});
    
		return this.http.get(URL, options).toPromise()
			.then(resp => resp.json())
			.catch(this.handleError)
	}
  
	getLocationById(id: number) {
		let options = new RequestOptions({
			headers: this.getHeaders()
		});
    
		return this.http.get(URL + id, options).toPromise()
			.then(resp => resp.json())
			.catch(this.handleError)
	}

	addLocation(location) {
		let options = new RequestOptions({
			headers: this.getHeaders()
		});
    
		return this.http.post(URL, location, options).toPromise()
			.then(resp => resp.json())
			.catch(this.handleError)
	}
    
	removeLocation(id) {
		let options = new RequestOptions({
			headers: this.getHeaders()
		});
    
		return this.http.delete(URL + id, options).toPromise()
			.then()
			.catch(this.handleError)
	}
  
	updateLocation(location) {
		let options = new RequestOptions({
			headers: this.getHeaders()
		});
    
		return this.http.put(URL, location, options).toPromise()
			.then()
			.catch(this.handleError)
	}

	private getHeaders() {
		return new Headers({
			'Authorization': `TenantSecret ${USER_ID},${USER_SECRET}` 
		});
	}

	private handleError(err) {
		let errMsg = (err.message) 
			? err.message 
			: err.status ? `${err.status}: ${err.statusText}` : 'Server error';
		
		
		console.error(errMsg);
		
		return Promise.reject(errMsg);   
	}
}