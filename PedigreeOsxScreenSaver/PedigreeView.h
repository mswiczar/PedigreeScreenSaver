//
//  PedigreeView.h
//  Pedigree
//
//  Created by Moises Swiczar on 3/10/10.
//  Copyright (c) 2010, Apple Inc. All rights reserved.
//

#import <ScreenSaver/ScreenSaver.h>
#import <Cocoa/Cocoa.h>
#import <QTKit/QTKit.h>

@interface PedigreeView : ScreenSaverView 
{
	BOOL realyisPreview;
	NSImageView * theimageview;
	
	NSMutableArray * thearray;
	QTMovieView* theqtView ;
	QTMovie* theqt;
	NSTimer * thetimer;
	NSTimer * thetimer2;
	NSTimer *  thetimer3;
	NSUInteger  theactual;
	NSURL				*soundFileURL;
	BOOL playing;
	
}
@property (nonatomic, retain)	NSURL				*soundFileURL;
@end



