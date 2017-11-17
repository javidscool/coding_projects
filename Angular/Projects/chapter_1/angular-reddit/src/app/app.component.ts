import { Component } from '@angular/core';
import { Article } from './article/article.model';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {

	articles: Article[]; // <-- component property

	constructor() {
		this.articles = [
			new Article('Angular 2', 'http://angular.io', 3),
			new Article('Fullstack', 'http://fullstack.io', 2),
			new Article('Angular Homepage', 'http://angular.io', 1),
		];
	}

	addArticle(title: HTMLInputElement, link: HTMLInputElement): boolean {
		//first print out the article we’re adding
		console.log(`Adding article title: ${title.value} and link: ${link.value}`);
	
		//then add the article to the articles array
		this.articles.push(new Article(title.value, link.value, 0));

		//lastly reset the fields and return false to stop propagation of click event
		title.value = '';
		link.value = '';
		return false;
	}
	
	sortedArticles(): Article[] {
		return this.articles.sort((a: Article, b: Article) => b.votes - a.votes);
	}

}
