Header header
uint8 address
uint8 remote_address
# The state is [p_x, p_y, p_z, v_x, v_y, v_z], where p_i and v_i are the position and velocity in dimension i
float32[] state
# A 6x6 matrix (i.e. 36 elements) giving the covariance of the state (storage order is row-major)
float32[] covariance
