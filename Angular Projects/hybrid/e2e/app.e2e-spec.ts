import { HybridPage } from './app.po';

describe('hybrid App', () => {
  let page: HybridPage;

  beforeEach(() => {
    page = new HybridPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});
