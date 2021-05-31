using UnityEngine;
using UnityEngine.InputSystem;


public class CameraController : MonoBehaviour, PlayerInputAction.ICameraActions
{
    public Transform parent;
    private PlayerInputAction playerInputAction;
    private float xRotation;
    [Range(-70f, 0f)]
    public float xMin = -60f;
    [Range(0f, 70f)]
    public float xMax = 65f;

    private void Awake()
    {
        xRotation = 0f;
        parent = transform.parent;
        playerInputAction = new PlayerInputAction();
        playerInputAction.Camera.SetCallbacks(this);
    }

    void OnEnable()
    {
        playerInputAction.Camera.Enable();
    }

    void OnDisable()
    {
        playerInputAction.Camera.Disable();
    }

    public void OnRotate(InputAction.CallbackContext context)
    {

        if (context.performed)
        {
            Vector2 axis = context.ReadValue<Vector2>();

            xRotation += axis.y;
            xRotation = Mathf.Clamp(xRotation, xMin, xMax);
            transform.localRotation = Quaternion.Euler(Vector3.left * xRotation);
            parent.Rotate(Vector3.up * axis.x);
        }
 
    }
}