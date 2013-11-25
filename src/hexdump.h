/*
 * hexdump.h
 *
 * Created by Sean Starkey <sean@seanstarkey.com>
 *
 * NO COPYRIGHT - THIS IS 100% IN THE PUBLIC DOMAIN
 *
 * The original version is available at:
 *    https://github.com/SeanStarkey/chexdump
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR(S) OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _HEXDUMP_H
#define _HEXDUMP_H

#ifdef __cplusplus
extern "C" {
#endif

void ptrhexdumpToFile(FILE* output,
                      unsigned char* input, unsigned long inputLength);
void ptrhexdumpToStr(char* output, unsigned long outputLength,
                     unsigned char* input, unsigned long inputLength);
void filehexdumpToFile(FILE* output,
                       FILE* input, long offset, int whence,
                       unsigned long inputLength);
void filehexdumpToStr(char* output, unsigned long outputLength,
                      FILE* input, long offset, int whence,
                      unsigned long inputLength);

#ifdef __cplusplus
}
#endif

#endif
