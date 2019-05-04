// SuperFastHash, Copyright (c) Paul Hsieh
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "PiEnginePreCompiled.h"
#include "Hash.h"

#define get16bits(d) (*((const uint16*) (d)))


	uint32 SuperFastHash(const char* data, int32 len)
	{
		uint32 hash = len, tmp;
		int32 rem;
		
		if (len <= 0 || data == NULL) return 0;
		
		rem = len & 3;
		len >>= 2;
		
		/* Main loop */
		for (; len > 0; len--)
		{
			hash += get16bits(data);
			tmp = (get16bits(data + 2) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			data += 2 * sizeof(uint16);
			hash += hash >> 11;
		}
		
		/* Handle end cases */
		switch (rem)
		{
		case 3: hash += get16bits(data);
			hash ^= hash << 16;
			hash ^= ((signed char)data[sizeof(uint16)]) << 18;
			hash += hash >> 11;
			break;
		case 2: hash += get16bits(data);
			hash ^= hash << 11;
			hash += hash >> 17;
			break;
		case 1: hash += (signed char)*data;
			hash ^= hash << 10;
			hash += hash >> 1;
		}
		
		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;
		
		return hash;
	}

	uint32 Hash(const char* data, int32 len)
	{
		return SuperFastHash(data, len);
	}

	uint32 Hash(const char* data)
	{
		return SuperFastHash(data, static_cast<int32>(strlen(data)));
	}

	uint32 Hash(const string& data, int32 len)
	{
		return SuperFastHash(data.c_str(), len);
	}

	uint32 Hash(const string& data)
	{
		return SuperFastHash(data.c_str(), static_cast<uint32>(data.size()));
	}


