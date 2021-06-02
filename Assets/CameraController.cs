using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;


public class CameraController : MonoBehaviour, PlayerInputAction.ICameraActions
{
    //ZoomTest
    public Transform player;
    public LayerMask layerMask;

    public Transform parent;
    private PlayerInputAction playerInputAction;
    private float xRotation;
    public float zoom = 1f;
    [Range(0f, 1f)]
    public float sensitivity = 0.2f;
    [Range(-120f, 0f)]
    public float xMin = -90f;
    [Range(0f, 100f)]
    public float xMax = 90f;
    [SerializeField] private Camera myCamera;
    public GameObject newObj;

    [SerializeField] private float distance;
    [SerializeField] private float myDistance;

    private bool processing;
    private void Awake()
    {
        processing = false;
        xRotation = 0f;
        parent = transform.parent;
        player = parent.parent;
        playerInputAction = new PlayerInputAction();
        playerInputAction.Camera.SetCallbacks(this);

        myCamera = GetComponent<Camera>();
    }

    void Start()
    {
        newObj = new GameObject();
        newObj.transform.SetParent(player);

        newObj.transform.position = transform.position;
        newObj.transform.LookAt(player);

        myDistance = Vector3.Distance(newObj.transform.position, player.position);


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
            transform.localRotation = Quaternion.Euler(Vector3.left * xRotation * sensitivity);
            parent.Rotate(Vector3.up * axis.x * sensitivity);

        }

        newObj.transform.position = transform.position;
        newObj.transform.LookAt(player);

        Vector3 fwd = newObj.transform.TransformDirection(Vector3.forward);

        distance = Vector3.Distance(newObj.transform.position, player.position);

        RaycastHit hit;

        if (Physics.Raycast(newObj.transform.position, fwd, out hit, distance, layerMask) && !processing)
        {
            processing = true;
            StartCoroutine("Test", fwd);

   
        }

    }
    IEnumerator Test(Vector3 fwd)
    {
        RaycastHit hit;

        Debug.Log("Start");

        while (Physics.Raycast(newObj.transform.position, fwd, out hit, distance, layerMask))
        {
            newObj.transform.position = transform.position;
            newObj.transform.LookAt(player);

            fwd = newObj.transform.TransformDirection(Vector3.forward);

            newObj.transform.Translate(Vector3.forward * Time.fixedDeltaTime * 5f);
            transform.position = newObj.transform.position;

            yield return new WaitForFixedUpdate();
        }

        Debug.Log("End");
        processing = false;

        yield return null;
    }



    public void OnZoomInOut(InputAction.CallbackContext context)
    {
        newObj.transform.position = transform.position;
        newObj.transform.LookAt(player);

        float middleMouseY = context.ReadValue<float>();
        if (middleMouseY > 0)
        {
            newObj.transform.localPosition += Vector3.forward;
            transform.position = newObj.transform.position;
            myDistance = Vector3.Distance(newObj.transform.position, player.position);
        }

        else if (middleMouseY < 0)
        {
            newObj.transform.localPosition -= Vector3.forward;
            transform.position = newObj.transform.position;
            myDistance = Vector3.Distance(newObj.transform.position, player.position);
        }
    }
}