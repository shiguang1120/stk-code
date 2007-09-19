#ifdef HAVE_GHOST_REPLAY

#include <cassert>


#include "replay_base.hpp"
#include "replay_buffers.hpp"


ReplayBuffers::ReplayBuffers()
: m_number_cars(0),
  m_BufferFrame(),
  m_BufferKartState()
{
}

ReplayBuffers::~ReplayBuffers()
{
    destroy();
}

void ReplayBuffers::destroy()
{
    m_BufferFrame.destroy();
    m_BufferKartState.destroy();
    m_number_cars = 0;
}

bool ReplayBuffers::init( unsigned int number_cars, 
                          size_t number_preallocated_frames )
{
    m_number_cars = number_cars;

    if( !m_BufferFrame.init( number_preallocated_frames ) ) return false;
    if( !m_BufferKartState.init( number_preallocated_frames, number_cars ) ) return false;

    return true;
}

ReplayFrame* 
ReplayBuffers::getNewFrame()
{
    // make sure initialization was called properly
    assert( m_BufferFrame.getNumberObjectsUsed() == m_BufferKartState.getNumberArraysUsed() );

    if( !isHealthy() ) return false;

    ReplayFrame* frame = m_BufferFrame.getNewObject();
    if( !frame ) return NULL;

    // get current karts-array
    ReplayKartState* karts_array = m_BufferKartState.getNewArray();
    if( !karts_array ) return NULL;
    
    frame->p_kart_states = karts_array;

    return frame;
}

ReplayFrame const* 
ReplayBuffers::getFrame( size_t frame_index ) const
{
    return m_BufferFrame.getObjectAt( frame_index );
}

bool ReplayBuffers::saveReplayHumanReadable( FILE *fd ) const
{
    if( !isHealthy() ) return false;

    if( fprintf( fd, "frames: %u\n", getNumberFramesUsed() ) < 1 ) return false;
    
    unsigned int frame_idx, kart_idx;
    ReplayFrame const *frame;
    ReplayKartState const *kart;
    for( frame_idx = 0; frame_idx < getNumberFramesUsed(); ++frame_idx )
    {
        frame = getFrame( frame_idx );
        if( fprintf( fd, "frame %u  time %f\n", frame_idx, frame->time ) < 1 ) return false;

        for( kart_idx = 0; kart_idx < m_number_cars; ++kart_idx )
        {
            kart = frame->p_kart_states + kart_idx;

            if( fprintf( fd, "\tkart %u: %f,%f,%f,%f,%f,%f\n", kart_idx, 
                     kart->position.xyz[0], kart->position.xyz[1], kart->position.xyz[2],
                     kart->position.hpr[0], kart->position.hpr[1], kart->position.hpr[2] ) < 1 ) return false;   
        }
    }

    return true;
}

bool ReplayBuffers::loadReplayHumanReadable( FILE *fd, size_t number_cars )
{
    size_t frames;
    if( fscanf( fd, "frames: %u\n", &frames ) != 1 ) return false;

    if( !init( number_cars, frames ) ) return false;

    unsigned int frame_idx, kart_idx, tmp;
    ReplayFrame *frame;
    ReplayKartState *kart;
    for( frame_idx = 0; frame_idx < getNumberFramesUsed(); ++frame_idx )
    {
        frame = m_BufferFrame.getObjectAt( frame_idx );
        assert( frame );
        if( fscanf( fd, "frame %u  time %f\n", &tmp, &frame->time ) != 2 ) return false;

        frame->p_kart_states = m_BufferKartState.getArrayAt( frame_idx );
        assert( frame->p_kart_states );

        for( kart_idx = 0; kart_idx < number_cars; ++kart_idx )
        {
            kart = frame->p_kart_states + kart_idx;

            if( fscanf( fd, "\tkart %u: %f,%f,%f,%f,%f,%f\n", &tmp, 
                     &kart->position.xyz[0], &kart->position.xyz[1], &kart->position.xyz[2],
                     &kart->position.hpr[0], &kart->position.hpr[1], &kart->position.hpr[2] ) != 7 ) return false;   
        }
    }

    return true;
}



#endif // HAVE_GHOST_REPLAY
