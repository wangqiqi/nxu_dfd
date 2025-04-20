#include "nxu_dfd_api.h"
#include "nxu_dfd_gconfig.h"
#include "nxu_dfd_utils.h"

using namespace nxu_dfd_utils;

// 算法句柄结构体
typedef struct _nxu_dfd_handle_s
{
    CNXUDFDUtils *__dfdobj;
} NXU_DFD_Handle_S;

/**
 * @brief 创建算法句柄，加载算法模型，只用创建一次，就可以循环使用，每一次创建对应一次销毁
 *
 * @param  phDFD            算法句柄
 * @return NXU_EXPORTS
 */
NXU_STATUS_CODE_E NXU_DFD_Create(NXU_HANDLE *phDFD)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建dfd实例
    NXU_DFD_Handle_S *hDFD = new NXU_DFD_Handle_S;
    if (NULL == hDFD)
    {
        return NXU_FAILURE;
    }

    hDFD->__dfdobj = new CNXUDFDUtils();
    if (NULL == hDFD->__dfdobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phDFD = (NXU_HANDLE)hDFD;

    return s32Ret;
}


NXU_STATUS_CODE_E NXU_DFD_CreateWithModel(NXU_HANDLE* phDFD,
    char* modelPath)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建dfd实例
    NXU_DFD_Handle_S* hDFD = new NXU_DFD_Handle_S;
    if (NULL == hDFD)
    {
        return NXU_FAILURE;
    }

    hDFD->__dfdobj = new CNXUDFDUtils(modelPath);
    if (NULL == hDFD->__dfdobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phDFD = (NXU_HANDLE)hDFD;

    return s32Ret;
}


/**
 * @brief 销毁算法句柄，使用完后记得销毁算法句柄
 *
 * @param  hDFD             算法句柄
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_DFD_Destroy(NXU_HANDLE hDFD)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_DFD_Handle_S *pHandle = (NXU_DFD_Handle_S *)hDFD;
    if (NULL == pHandle)
    {
        return NXU_FAILURE;
    }

    if (NULL == pHandle->__dfdobj)
    {
        return NXU_FAILURE;
    }
    else
    {
        delete pHandle->__dfdobj;
        pHandle->__dfdobj = NULL;
    }

    delete pHandle;
    pHandle = NULL;

    return s32Ret;
}

/**
 * @brief 获取算法的版本号
 *
 * @param  pcLibVerStr      算法库版本号
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_DFD_GetLibVer(NXU_S8 *pcLibVerStr)
{

    // 获取版本号
    std::cout << "\n=============== LIB INFO =====================\n";
    std::cout << "PROJECT_NAME: " << DFD_PROJECT_NAME << std::endl;
    std::cout << "LIB_VERSION: " << DFD_VERSION << std::endl;
    std::cout << "LIB_COMMIT_MD5: " << DFD_COMMIT << std::endl;
    std::cout << "BUILD_DATETIME: " << DFD_DATETIME << std::endl;
    std::cout<< "==============================================\n";

    sprintf((char*) pcLibVerStr, "%s-%s-%s-%s", DFD_PROJECT_NAME, 
            DFD_VERSION, DFD_COMMIT, DFD_DATETIME);
            
    return NXU_SUCCESS;
}

/**
 * @brief 算法处理
 *
 * @param  hDFD             算法句柄
 * @param  pstImage         图像数据
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_DFD_Process(NXU_HANDLE hDFD, const NXU_IMAGE_S *pstImage)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    NXU_DFD_Handle_S *pHandle = (NXU_DFD_Handle_S *)hDFD;
    if (NULL == pHandle || NULL == pstImage || NULL == pstImage->pData)
    {
        return NXU_FAILURE;
    }

#ifdef SHOW_DEBUG_TIME
    double dtStart, dtEnd;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    // 处理图像
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__dfdobj->process(pstImage));
    if (s32Ret != NXU_SUCCESS)
    {
        return s32Ret;
    }

#ifdef SHOW_DEBUG_TIME
    dtEnd = __get_current_time_proc();
    std::cout << "++++++  In DFD API: Process Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    return s32Ret;
}

/**
 * @brief 获取结果
 *
 * @param  hDFD             算法句柄
 * @param  pstDFDResult     DFD结果
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_DFD_GetResults(NXU_HANDLE hDFD, NXU_DFD_Result_S *pstDFDResult)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_DFD_Handle_S *pHandle = (NXU_DFD_Handle_S *)hDFD;
    if (NULL == pHandle || NULL == pstDFDResult)
    {
        return NXU_FAILURE;
    }

    // 获取结果，通过参数返回出来
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__dfdobj->get_results(pstDFDResult));

    return s32Ret;
}
