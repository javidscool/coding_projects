import { SpotifyService, SPOTIFY_ID, SPOTIFY_SECRET } from './spotify.service';

export const spotifyInjectables: Array<any> = [
	{provide: SpotifyService, useClass: SpotifyService},
	{provide: SPOTIFY_ID, useValue: SPOTIFY_ID},
	{provide: SPOTIFY_SECRET, useValue: SPOTIFY_SECRET}
];