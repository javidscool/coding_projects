import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ProductsRowComponent } from './products-row.component';

describe('ProductsRowComponent', () => {
  let component: ProductsRowComponent;
  let fixture: ComponentFixture<ProductsRowComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ProductsRowComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ProductsRowComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
