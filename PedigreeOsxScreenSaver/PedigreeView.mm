//
//  PedigreeView.m
//  Pedigree
//
//  Created by Moises Swiczar on 3/10/10.
//  Copyright (c) 2010, Apple Inc. All rights reserved.
//

#import "PedigreeView.h"

#import "Recorder.h"




@implementation PedigreeView

@synthesize soundFileURL;

-(NSUInteger) getnextrandom:(NSMutableArray*) arraynew
{
	
	return SSRandomIntBetween (0,[arraynew count]-1);
	
	
}



-(void) mix:(NSMutableArray*) thearraytodoit;
{
	NSMutableArray * arraynew;
	arraynew= [[NSMutableArray alloc]initWithArray:thearraytodoit];
	
	[thearraytodoit removeAllObjects];
	NSUInteger zzz;
	NSUInteger zzztotal=[arraynew count];
	NSUInteger i;
	for (zzz=0; zzz< zzztotal; zzz++)
	{
		i = [self getnextrandom:arraynew];
		//NSLog(@"%d",i);
		[thearraytodoit addObject:[arraynew objectAtIndex:i]];
		[arraynew removeObjectAtIndex:i];
	}
}


-(void) stopRecord
{
	stopRecording();
	NSLog(@"Stop Record");
}

-(void) startRecord
{
	startRecording();
	
	
}


-(IBAction)clickRecord:(id)aobj
{
	[self startRecord];
}



- (id)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview
{
    self = [super initWithFrame:frame isPreview:isPreview];
    if (self) {
		srand ( time(NULL) );
		playing=NO;

		
		
		
		
		theactual=0;
		srand ( time(NULL) );
		NSBundle *bundle;
		NSString *path;
		
		bundle = [NSBundle bundleForClass: [self class]];
		thearray= [[NSMutableArray alloc] init];
		
		path = [bundle pathForResource: @"1"  ofType: @"mov"];
		[thearray addObject:path];
		
		path = [bundle pathForResource: @"2"  ofType: @"mov"];
		[thearray addObject:path];
		
		path = [bundle pathForResource: @"3"  ofType: @"mov"];
		[thearray addObject:path];
		
		path = [bundle pathForResource: @"4"  ofType: @"mov"];
		[thearray addObject:path];
		
		path = [bundle pathForResource: @"5"  ofType: @"mov"];
		[thearray addObject:path];
		
			[self clickRecord:self];
		
		//[self startRecord];
		
	    }
    return self;
}

- (void)startAnimation
{
    [super startAnimation];
}

- (void)stopAnimation
{
    [super stopAnimation];
}

- (void)drawRect:(NSRect)rect
{
    [super drawRect:rect];
}


-(void)show:(id)aobj
{
	[self addSubview:theqtView];
	
	
}


-(void) replay:(id)aobj
{
	playing=YES;
	if(theactual < [thearray count]-1)
	{
		theactual++;
	}
	else
	{
		theactual=0;
		
	}
	[theqtView removeFromSuperview];
	[theqt release];
	theqt= [[QTMovie alloc] initWithFile:[thearray objectAtIndex:theactual] error:nil];
	[theqtView setMovie:theqt];
	[theqtView play:self];
	thetimer2 = [NSTimer scheduledTimerWithTimeInterval:	0.6		// seconds
												 target:		self
											   selector:	@selector (show:)
											   userInfo:	self		// makes the currently-active audio queue (record or playback) available to the updateBargraph method
												repeats:	NO];
	
	
}

-(void) checkea:(id) aobj
{
	
	
	if((playing==NO) && (silvo()==1))
	{
		playing=YES;
		[self replay:self];
		
	}
	
	QTTime thetime;
	QTTime thetimeMax =  [theqt duration];
	
	thetime = theqt.currentTime;
	NSLog(@"%d" , thetime.timeValue);
	NSLog(@"%d" , thetimeMax.timeValue);
	
	
	if (thetimeMax.timeValue == thetime.timeValue)
	{
		resetsilvo();
		playing=NO;
	}
	
	
}
-(IBAction) clickaction:(id)aobj
{
	[self mix: thearray];
	NSRect arect = [self frame];
	//arect.size.height=480; 
	//arect.size.width=720; 
	
	theqtView = [[QTMovieView alloc] initWithFrame:arect];
	
	theqt= [[QTMovie alloc] initWithFile:[thearray objectAtIndex:theactual] error:nil];
	[theqtView setMovie:theqt];
	[theqtView setControllerVisible:NO];
	
	NSImage * theimage;
	NSBundle *bundle;
	NSString *path;
	
	bundle = [NSBundle bundleForClass: [self class]];
	path = [bundle pathForResource: @"P1"  ofType: @"png"];
	
	theimage = [[NSImage alloc] initWithContentsOfFile:path];
	
	theimageview = [[NSImageView alloc] initWithFrame:[self frame]];
	[theimageview setImage:theimage];
	[theimageview setImageScaling:NSScaleToFit];
	
	[self addSubview:theimageview];
	
	
	[self addSubview:theqtView];
	
	
	
	[[theqtView movie] play];
//	[theqtView play:theqtView];
	playing=YES;
	thetimer3 = [NSTimer scheduledTimerWithTimeInterval:	 0.3// seconds
												 target:		self
											   selector:	@selector (checkea:)
											   userInfo:	self		// makes the currently-active audio queue (record or playback) available to the updateBargraph method
												repeats:	YES];
}



- (void)animateOneFrame
{
	[self clickaction:self];

	[self stopAnimation];
	return;
}



-(IBAction)clickStop:(id)aobj;
{
	[self stopRecord];
}



- (BOOL)hasConfigureSheet
{
    return NO;
}

- (NSWindow*)configureSheet
{
    return nil;
}


@end
