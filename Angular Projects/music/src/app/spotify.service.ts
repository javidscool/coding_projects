import { Injectable, Inject } from '@angular/core';
import { Http, Response, RequestOptions, Headers, URLSearchParams } from '@angular/http';
import { Observable } from 'rxjs/Observable';
import 'rxjs/Rx';

//SpotifyService works querying the Spotify Web API

export const SPOTIFY_ID: string = 'ad2f9f2a95e64a22a088056deda9a623';
export const SPOTIFY_SECRET: string = '84bef3adede94dfda207f599a024a81e';

@Injectable()
export class SpotifyService {
	static BASE_URL = 'https://api.spotify.com/v1';
	static TOKEN_API = 'https://accounts.spotify.com/api/token';

	private data: Object;
	private accessToken: string;
	
	constructor(private http: Http,
				@Inject(SPOTIFY_ID) private spotifyId: string,
				@Inject(SPOTIFY_SECRET) private spotifySecret: string) { 
				
		this.requestAuth();
	}
	
	private requestAuth(): void{
		const headers: Headers = new Headers();
		headers.append('Accept', 'application/json');
		headers.append('Content-Type', 'application/x-www-form-urlencoded');
		//comment out to make Jasmine testing work because Jasmine doesn't like btoa()
		headers.append('Authorization', 'Basic ' + btoa(this.spotifyId + ":" + this.spotifySecret));
		
		const opts: RequestOptions = new RequestOptions();
		opts.headers = headers;
		this.http.post(`${SpotifyService.TOKEN_API}`, "grant_type=client_credentials", opts)
		.subscribe((res: Response) => {
			this.data = res.json();
			this.accessToken = this.data['access_token'];
			//console.log(res.json());
			//console.log(this.data['access_token']);
		});
		
	}

	query(URL: string, params?: Array<string>): Observable<any[]> {
		let queryURL = `${SpotifyService.BASE_URL}${URL}`;
		
		if (params) {
			queryURL = `${queryURL}?${params.join('&')}`;
		}
		
		const headers: Headers = new Headers();
		headers.append('Authorization', 'Bearer ' + this.accessToken);
		
		const opts: RequestOptions = new RequestOptions();
		opts.headers = headers;

		return this.http.request(queryURL, opts).map((res: any) => res.json());
	}

	search(query: string, type: string): Observable<any[]> {
		return this.query(`/search`, [
			`q=${query}`,
			`type=${type}`
		]);
	}

	searchTrack(query: string): Observable<any[]> {
		return this.search(query, 'track');
	}

	getTrack(id: string): Observable<any[]> {
		return this.query(`/tracks/${id}`);
	}

	getArtist(id: string): Observable<any[]> {
		return this.query(`/artists/${id}`);
	}

	getAlbum(id: string): Observable<any[]> {
		return this.query(`/albums/${id}`);
	}
}

export const SPOTIFY_PROVIDERS: Array<any> = [
	{ provide: SpotifyService, useClass: SpotifyService }
];
