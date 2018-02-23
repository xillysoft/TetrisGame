//
//  GameBoard.m
//  game_tetris
//
//  Created by zhaoxiaojian on 2/19/18.
//  Copyright © 2018 Zhao Xiaojian. All rights reserved.
//

#import "GameBoard.h"

@implementation GameBoard

-(instancetype)init
{
    self = [super init];
    self.state = STATE_INIT;
    return self;
}

-(void)initializeGameBoardWithRows:(NSInteger)rows columns:(NSInteger)columns
{
    NSAssert(_state==STATE_INIT
             , @"Wrong game state. Current game state=%lu, %lu expected", _state, STATE_INIT);
    _rows = rows;
    _columns = columns;
    //TODO: initialize the game board data structure
    //
    _block = nil;
    
    self.state = STATE_READY;
    [self.delegate gameBoard:self didInitializeWithRows:rows columns:columns];
}

-(BOOL)generateBlock:(Block *)block atRow:(NSInteger)row column:(NSInteger)column
{
    NSAssert(_state==STATE_READY
             , @"Wrong game state. Current game state=%lu, %lu expected", _state, STATE_READY);
    BOOL canGenerateBlock;
    //TODO: determine whether the *block* can be generated at given *position*
    //
    if( ! canGenerateBlock ){
        self.state = STATE_DEAD;
        [self.delegate block:block didFailToGenerateAtRow:row column:column];
        return NO;
    }else{
        //save the current block info
        _block = block;
        _blockRow = row;
        _blockColumn = column;
        
        self.state = STATE_BLOCK_GENERATED;
        [self.delegate block:block didGenerateAtRow:row column:column];
        return YES;
    }
}

-(BOOL)dropDownBlock
{
    NSAssert(_state==STATE_BLOCK_GENERATED
             || _state==STATE_BLOCK_DROPPED_DOWN
             , @"Wrong game state. Current game state=%lu, %lu expected", _state, STATE_BLOCK_GENERATED);
    
    BOOL blockReachedBottom;
    //TODO: determine whether the block reached bottom, that is, it could not dropdown any more.
    //
    if( blockReachedBottom ){
        self.state = STATE_BLOCK_REACHED_BOTTOM;
        [self.delegate block:_block didReachBottomToRow:_blockRow column:_blockColumn];
        return NO;
    }else{
        NSInteger rowDropped, columnDropped;
        rowDropped = _blockRow + 1;
        columnDropped = _blockColumn;
        
        _blockRow = rowDropped;
        _blockColumn = columnDropped;
        self.state = STATE_BLOCK_DROPPED_DOWN;
        [self.delegate block:_block didDropDownToRow:rowDropped column:columnDropped];
        return YES;
    }
}

-(void)mergeBlock
{
    NSAssert(_state==STATE_BLOCK_REACHED_BOTTOM
             , @"Wrong game state. Current game state=%lu, %lu expected", _state, STATE_BLOCK_REACHED_BOTTOM);
    
    self.state = STATE_MERGING_BLOCK;
    [self.delegate block:_block willMergeAtRow:_blockRow column:_blockColumn];
    
    //TODO: merge *block* into game board at *position*.
    //
    _block = nil;
    self.state = STATE_READY;
    [self.delegate block:_block didMergeAtRow:_blockRow column:_blockColumn];
    
}

@end
