/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * ******************************************************************************
 * This program is part of the source code provided with
 * "Linux Systems Programming Essentials" Course offered by TECHVEDA(c) 2020 - 2022
 * Course page : https://www.techveda.org/linuxsysprg
 * Trainer:  Raghu Bharadwaj
 * ******************************************************************************
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <excl.h>

/* 
 * Both counters are initialized with zero
 * 
 */
void lock_init(tlock_t * lck)
{
	lck->seq = 0;
	lck->lock = 0;
}

/* 
 * Each contending thread reads current value of seq counter, which is
 * its token to own the lock. It then polls on the lock counter until
 * its turn arrives.
 * When both 'lock' and 'ticket' counters match function returns and caller 
 * thread own's the lock.
 */ 
 
void lock(tlock_t * lck)
{
	int myticket;
	myticket = __sync_fetch_and_add(&lck->seq, 1);
	while(lck->lock != myticket);
}

/* void unlock(lock_t *lck)
   arg1 : address of lock instance
 * Unlock() will increment lock counter by 1, which causes lock to be passed 
 * to next owner with an appropriate ticket(token).
 */
void unlock(tlock_t * lck)
{
	lck->lock = lck->lock + 1;
}
