//Copyright(c) 2024 gdemers
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once

#include "DemoExpression.hh"

class UDotProduct : public UDemoExpression
{
	//	math proofs : where do they come from ?
	//
	//		1. (a * b) = ||a|| * ||b|| * cos(angle)
	//		2. (a * b) = (a.x * b.x) + (a.y * b.y) + (a.z * b.z)
	//
	//		reminder, we can unfold the above from the law of cosine : c^2 = a^2 + b^2 - 2(||a|| * ||b|| * cos(angle))
	// 
	//	how to get law of cosines ?
	//	
	//	... (drawing a non-right triangle is very hard with 45* fwd,back slash)
	// 
	//	TODO!
	//
	//		proof #1 :
	// 
	//				using law of cosine to find the missing edge in a non-right triangle, we can determine that the missing edge equate => c^2 == (a - b)^2
	//
	//				(a - b)^2 => a^2 + b^2 - 2(a * b), if we subtitute : a^2 + b^2 - 2(a * b) = a^2 + b^2 - 2(||a|| * ||b|| * cos(angle))
	// 
	//		solve :
	// 
	//				step 1. substract both sides	=> (a^2 + b^2) - (a^2 + b^2) = 0;	remaining => -2	(a * b) = -2(	||a|| * ||b|| * cos(angle))
	//				step 2. divide both sides by -2	=>									remaining =>	(a * b) =		||a|| * ||b|| * cos(angle)
	// 
	//			proof #1 - (a * b) = ||a|| * ||b|| * cos(angle)
	//
	//		result :
	// 
	//				Here, we get our dot product equation from the derivation of the above! However, this calculation cost two sqrt (square root) which are expensive.
	//
	//		proof #2 : 
	// 
	//				using the same law from above, we can approach our vectors differently and compute their magintude in order to work around the drawbacks of proof #1.
	// 
	//		solve :
	//
	//				step 1. compute c^2, by calculating the magnitude of each vector and try removing the need to sqrt => (a - b)^2
	// 
	//				long form : sqrt((a.x - b.x)^2 + (a.y - b.y)^2 + (a.z - b.z)^2)^2 => so, sqrt and ^2 cancel out in the end
	//
	//				resulting in : (a.x - b.x)^2 + (a.y - b.y)^2 + (a.z - b.z)^2
	//
	//				and unfolding into : a.x^2 + b.x^2 - 2(a.x * b.x) + a.y^2 + b.y^2 - 2(a.y * b.y) + a.z^2 + b.z^2 - 2(a.z * b.z)
	// 
	//				which equate to : ||a||^2 + ||b||^2 - 2((a.x * b.x) + (a.y * b.y) * (a.z * b.z)), now we can solve for the other side, and complete the proof.
	//
	//				step 2. compute the magnitude of vector on the right side => a^2 + b^2 - 2(||a|| * ||b|| * cos(angle))
	// 
	//				long form : sqrt(a.x^2 + a.y^2 + a.z^2)^2 + sqrt(b.x^2 + b.y^2 + b.z^2)^2 => so, sqrt and ^2 cancel out in the end
	//
	//				resulting in : ||a||^2 + ||b||^2 - 2(||a|| * ||b|| * cos(angle))
	// 
	//		Finally, we can solve our proof :
	// 
	//				||a||^2 + ||b||^2 - 2((a.x * b.x) + (a.y * b.y) * (a.z * b.z)) = ||a||^2 + ||b||^2 - 2(||a|| * ||b|| * cos(angle))
	//
	//				step 3. substract	=> ||a||^2 + ||b||^2 on each side.
	//				step 4. factor out	=> ||a|| * ||b|| * cos(angle) into the dot product => (a * b)
	// 
	//				which writes as follow :
	//
	//					- 2((a.x * b.x) + (a.y * b.y) * (a.z * b.z)) = - 2(||a|| * ||b|| * cos(angle)) /*or write (a * b)*/
	// 
	//				step 5. divide by -2 resulting in :
	// 
	//					proof #2 - (a * b) = (a.x * b.x) + (a.y * b.y) * (a.z * b.z)
	//
	//		result :
	// 
	//				Here, we get our dot product equation from the derivation of the law of cosines without the drawbacks of calculating the sqrt of vector A, B.
public:
	virtual std::size_t Size() const override { return sizeof(UDotProduct); };
	virtual void Tick() override;
	virtual void ApplicationDraw() override;
	virtual void ImGuiDraw() override;
};