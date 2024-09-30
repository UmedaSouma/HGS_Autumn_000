//===========================================================================================================================================================
// 
// result.cppÇÃÉwÉbÉ_Å[
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "main.h"
#include "scene.h"

class CResult :public CScene
{
public:
	CResult();
	~CResult()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:

};