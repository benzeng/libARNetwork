/**
 *  @file ARNETWORK_Manager.h
 *  @brief network manager allow to send data acknowledged or not.
 *  @date 05/18/2012
 *  @author maxime.maitre@parrot.com
**/

#ifndef _NETWORK_MANAGER_PRIVATE_H_
#define _NETWORK_MANAGER_PRIVATE_H_

#include "ARNETWORK_IOBuffer.h"
#include "ARNETWORK_Sender.h"
#include "ARNETWORK_Receiver.h"
#include <libARNetwork/ARNETWORK_Manager.h>

#define ARNETWORK_MANAGER_ACK_ID_OFFSET 128 /**< offset to add to the ID number of the IOBuffer to get the ID of its acknowledgement IOBuffer */

/**
 *  @brief get the identifier of the output buffer storing the acknowledgment for an output buffer storing data acknowledged.
 *  @param[in] ID identifier of the output buffer waiting an acknowledgment.
 *  @return identifier of the output buffer storing the acknowledgment.
**/
static inline int ARNETWORK_Manager_IDOutputToIDAck(int ID)
{
    return ID + ARNETWORK_MANAGER_ACK_ID_OFFSET;
}

/**
 *  @brief get the identifier of the output buffer storing data acknowledged for an output buffer storing acknowledgments.
 *  @param[in] ID identifier of the output buffer storing the acknowledgment.
 *  @return identifier of the output buffer waiting an acknowledgment.
**/
static inline int ARNETWORK_Manager_IDAckToIDInput(int ID)
{
    return ID - ARNETWORK_MANAGER_ACK_ID_OFFSET;
}

/**
 *  @brief network manager allow to send data acknowledged or not.
**/
struct ARNETWORK_Manager_t
{
    ARNETWORK_Sender_t *senderPtr; /**< Pointer on the sender */
    ARNETWORK_Receiver_t *receiverPtr; /**< Pointer on the receiver */
    ARNETWORK_IOBuffer_t **inputBufferPtrArr; /**< Address of the array storing the input buffer */
    ARNETWORK_IOBuffer_t **outputBufferPtrArr; /**< Address of the array storing the output buffer */
    int numberOfInput; /**< Number of input buffer */
    int numberOfOutput; /**< Number of output buffer */
    int numberOfInputWithoutAck; /**< Number of input buffer without the  buffers of acknowledgement */
    int numberOfOutputWithoutAck; /**< Number of output buffer without the  buffers of acknowledgement */
    ARNETWORK_IOBuffer_t **inputBufferPtrMap; /**< array storing the inputBuffers by their identifier */
    ARNETWORK_IOBuffer_t **outputBufferPtrMap; /**< array storing the outputBuffers by their identifier */
};

#endif /** _NETWORK_MANAGER_PRIVATE_H_ */

