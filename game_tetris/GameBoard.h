//
//  GameBoard.h
//  game_tetris
//
//  Created by zhaoxiaojian on 2/19/18.
//  Copyright © 2018 Zhao Xiaojian. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Block.h"

typedef NS_ENUM(NSUInteger, GameState) {
    STATE_INIT, /* The initial state */
    STATE_READY, /* New block could be generated in this state */
    STATE_BLOCK_GENERATED, /* Block generated and can be dropped down */
    STATE_BLOCK_DROPPED_DOWN, /* Block dropped down one or any steps */
    STATE_BLOCK_REACHED_BOTTOM, /* Block reached bottom and can be merged into the board */
    STATE_MERGING_BLOCK, /* Merging block into the board */
    STATE_DEAD, /* New block couldn't be generate */
};

@class Block;
@class GameBoard;

/*------- interface GameBoardDelegate -------*/
@protocol GameBoardDelegate

/**
 * The game board now has been initialized in STATE_READY state,
 * You may call -generateBlock:atRow:column: method to generate new block.
 */
-(void)gameBoard:(GameBoard *)gameBoard didInitializeWithRows:(NSInteger)rows columns:(NSInteger)columns;

/**
 * The block generated at given position.
 * You may call -dropDownBlock to try to get the block dropped down.
 */
-(void)block:(Block *)block didGenerateAtRow:(NSInteger)row column:(NSInteger)column;

/**
 * The block could not generate at given position, game state transitioned to STATE_DEAD.
 */
-(void)block:(Block *)block didFailToGenerateAtRow:(NSInteger)row column:(NSInteger)column;

/**
 * The block dropped down one or any steps.
 * You may call -dropDownBlock to try to get the block dropped down.
 */
-(void)block:(Block *)block didDropDownToRow:(NSInteger)row column:(NSInteger)column;

/**
 * The block reached to the bottom.
 * You may call -mergeBlock to merge the block into game board.
 */
-(void)block:(Block *)block didReachBottomToRow:(NSInteger)row column:(NSInteger)column;

/**
 * The block will be merged into the game board.
 */
-(void)block:(Block *)block willMergeAtRow:(NSInteger)row column:(NSInteger)column;

/**
 * The block merged into the game board and the state transitioned to STATE_READY.
 * You may call -generateBlock:atRow:column: to generate block.
 */
-(void)block:(Block *)block didMergeAtRow:(NSInteger)row column:(NSInteger)column;

@end


/*------ class GameBoard ---------*/
@interface GameBoard : NSObject

@property GameState state; //the initial state is STATE_INIT
@property id<GameBoardDelegate> delegate;

@property NSInteger rows;
@property NSInteger columns;

//the current block, may be nil.
@property Block *block;
@property NSInteger blockRow;
@property NSInteger blockColumn;

/**
 * Call this method to initialize the game board (must in STATE_INIT state).
 *
 * After game board initialized, the game board transitioned into STATE_READY state 
 * and the -gameBoard:didInitializeWithRows:columns: delegate method will get called.
 * You may call -generateBlock:atRow:column: method to generate new block in the delegate method.
 */
-(void)initializeGameBoardWithRows:(NSInteger)rows columns:(NSInteger)columns;

/*
 * Call this method to generate block (must in STATE_READY state).
 * That is, the -gameBoard:didInitializedWithRows:columns: or the -block:didMergeAtRow:column: delegate method get called.
 *
 * If the block given successfuly generated at specified position, the game board transitioned into STATE_BLOCK_GENERATED state (then transitioned to STATE_BLOCK_PENDING immediately),
 * and the -block:didGenerateAtRow:clumn: delegate method will get called. After that delegate method got called,
 * you may call -dropDownBlock method to get the block dropped down repeatedly.
 *
 * If the block fails to generate on gameboard at specified position, the game board transitioned into STATE_DEAD state and the -block:didFailToGenerateAtRow:column: delegate method.
 */
-(BOOL)generateBlock:(Block *)block atRow:(NSInteger)row column:(NSInteger)column;

/**
 * Call this method to make the block drop down (must in STATE_BLOCK_GENERATED or STATE_BLOCK_DROPPED_DOWN) state.
 *
 * If the block has been dropped down to the new position, the game board will transition into the STATE_BLOCK_DROPPED_DOWN state, and the -block:didDropDownToRow:column: delegate method will get called.
 * You may call -dropDownBlock in that delegate method again
 *
 * If the block could not drop down any more, the game board will transition into
 * the STATE_BLOCK_REACHED_BOTTOM state, and the -block:didReachBottomToRow:column: delegate method will get called.
 * You may call -mergeBlock:atRow:column: to merge the dropped block into the gameboard.
 *
 */
-(BOOL)dropDownBlock;

/**
 * Call this method to merge the block that is dropped down bottom (must in STATE_BLOCK_REACHED_BOTTOM state).
 *
 * Before the block has been merged into the game board, the game board will transition into the STATE_MERGING_BLOCK state and the -blockWillMergeAtRow:column: delegate method,
 * after the block has been merged into the game board, the game board will transition into the STATE_READY state
 * and the -block:didMergeAtRow:column: delegate method will get called, 
 * you may call -generateBlock:atRow:column: method to generate new block in that delegate method.
 */
-(void)mergeBlock;

@end



